#include <getopt.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "main.h"
#include "code.h"

#include <assert.h>

char *argv_0;

unsigned char *generate = NULL;
unsigned int deg_generate;
unsigned int alpha = 0;
unsigned int mod = 0;

enum{NONE = 0, ENCODE = 1, DECODE = 2, INFO = 3} doit = NONE;

void usage() {
	fprintf(stderr, "Использование: %s опции\n", argv_0);
	fprintf(stderr, "Программа реализующая БЧХ код; преобразует стандартный поток ввода.\n");
	fprintf(stderr, "\n");

	fprintf(stderr, "  Опции:\n");
	fprintf(stderr, "  \t-h, --help\tто, что вы сейчас читаете\n");
	fprintf(stderr, "  \t-V, --version\tверсия программы\n");
	fprintf(stderr, "  \t-i=\"-e -d\", --info=\"--encode --decode\"\tнапечатать информацию по коду\n");
	fprintf(stderr, "  \t-e, --encode\tкодирования\n");
	fprintf(stderr, "  \t-d, --decoding\tдекодирование\n");
	fprintf(stderr, "  \t-a, --alpha\tчисло\n\t\t\tкорень порождающего многочлена [в двойчном представлении]\n\t\t\t[g(a) == g(a^2) == ... == g(a^(d-1)) == 0, примитивный элемент поля GF(2^m)]\n");
	fprintf(stderr, "  \t-g, --generate-polynom\tстрока над {0, 1}\n\t\t\tпорождающий многочлен [g0g1...gn]\n\t\t\t[многочлен наименьшей степени имеющий корни g(a) == g(a^2) == ... == g(a^(d-1)) == 0]\n");
	fprintf(stderr, "  \t-f, --gf\tчисло\t\n\t\t\tнеприводимый многочлен [в двойчном представлении]\n\t\t\t[GF(2^m) ~~ GF(2)[x]/f(x) - поле разложения g(x)]\n");
	fprintf(stderr, "\n");

	fprintf(stderr, "  Пример:\n");
	fprintf(stderr, "\tcat file | %s -e -g 11101100101 -a 2 -f 19 > file.bch\n", argv_0);
	fprintf(stderr, "\n");
	
	fprintf(stderr, "  Змечание:\n");
	fprintf(stderr, "\tДлинна входящего потока должна быть кратна длинне кода; входящий поток должен состоять из ноликов и единичек.\n");
	fprintf(stderr, "\n");

	fprintf(stderr, "Об ошибках и предложениях сообщяйте на <artjcoder@gmail.ru>\n");

	exit(-1);
}

void version() {
	fprintf(stderr, "BCH coder version -.5\n");
	exit(-1);
}

void options(int argc, char **argv) {
	static char opts[] = "hViedg:a:f:";	// Описание коротких аргументов
	static struct option long_opts[] = {	// Описание длинных аркументов
		{"help", 0, NULL, 'h'},
		{"version", 0, NULL, 'V'},
		{"info", 0, NULL, 'i'},
		{"encode", 0, NULL, 'e'},
		{"decode", 0, NULL, 'd'},
		{"generate-polynom", 1, NULL, 'g'},
		{"alpha", 1, NULL, 'a'},
		{"gf", 1, NULL, 'f'}
	};		
	int long_index;				// "вспомогательная переменная" для getopt_long
	int opt;				// очередной прочитанный параметр

	char *endptr;				// "вспомогательная переменная для strtoul
	unsigned long _alpha;			// временная переменная для alpha
	unsigned long _mod;			// временная переменная для mod

	unsigned int i;
	
	opterr = 0;

	while( 1 ) {
		opt = getopt_long(argc, argv, opts, long_opts, &long_index);

		if( -1 == opt )
			break;
		else if( 'h' == opt )
			usage();
		else if( 'V' == opt )
			version();
		else if( 'i' == opt ) 
			doit = INFO;
		else if( 'e' == opt ) 
			doit |= ENCODE;
		else if( 'd' == opt )
			doit |= DECODE;
		else if( 'g' == opt ) {
			generate = optarg;
			deg_generate = strlen(generate) - 1;
			for(i = deg_generate; i != -1; --i)
				generate[i] -= '0';
					// @@ вообще-то мы портим глобальный параметр
		} else if( 'a' == opt ) {
			_alpha = strtoul(optarg, &endptr, 0);
			if( NULL == endptr || '\0' != *endptr ) {
				fprintf(stderr, "%s: alpha: %s\n", argv_0, strerror(errno));
				exit(-1);
			}
			alpha = _alpha;
		} else if( 'f' == opt ) {
			_mod = strtoul(optarg, &endptr, 0);
			if( NULL == endptr || '\0' != *endptr ) {
				fprintf(stderr, "%s: gf: %s\n", argv_0, strerror(errno));
				exit(-1);
			}
			mod = _mod;
		} else if( '?' == opt ) {
			fprintf(stderr, "%s: %s: ошибка в опции\n", argv_0, argv[optind - 1]);
								//@@ никто нам не гарантирует, что optind указывает на сл. параметр
		}
	}

	if( NONE == doit ) {
		fprintf(stderr, "%s: не понятно что делать [см. -h/--help]\n", argv_0);
		exit(-1);
	}
}

int main(int argc, char **argv) {
	struct bch_code_t *bch;		// Переменная для хранения "кода"

	unsigned int in_buf_size;	// Размер буфера ввода
	unsigned int out_buf_size;	// размер буфера вывода

	unsigned char *in_buf;		// Буфер ввода
	unsigned char *out_buf;		// Буфер вывода

	size_t rec;			// Результат выданый функциями ввода вывода

	unsigned int i;			// Вспомогательная переменная
	enum{INIT, IS_OPTION, IS_MEM, IS_READ, IS_WRITE, IS_EOF, IS_TAIL} state = INIT;

	enum{INIT, IS_OPTION, IS_MEM, IS_READ, IS_WRITE, IS_EOF, IS_TAIL} state = INIT;
	enum{false, true} ok = true;

	argv_0 = argv[0];

	if( ok ) {
		state = IS_OPTION;
		options(argc, argv);
		ok = (NULL != generate && 0 != alpha && 0 != mod);
	}
	if( ok ) 
		bch = new_bch_code(generate, deg_generate, mod, alpha);
		
	if( ok ) 
		if( doit == INFO ) {
			fprintf(stderr, "Информация по коду:\n");
			fprintf(stderr, "Неприводимый многочлен: %llu [%llx]\n", bch->_gf2n->_mod, bch->_gf2n->_mod);
			fprintf(stderr, "Корень порождающего многочлена: %u [%x]\n", bch->_alpha, bch->_alpha);

			fprintf(stderr, "Порождающий многочлен: ");

			for(i = 0; i <= bch->_degg; ++i)
				fprintf(stderr, "%c", bch->_g[i] + '0');
			fprintf(stderr, "\n");
			
			fprintf(stderr, "n: %u\nk: %u\nd: %u\nt: %u\n", bch->_n, bch->_k, bch->_d, bch->_t);
			exit(-1);
		}
		
	if( ok ) {
		if( doit&ENCODE ) {
			in_buf_size = bch->_k + 1;
			out_buf_size = bch->_n;
		} else {
			in_buf_size = bch->_n;
			out_buf_size = bch->_k + 1;
		}
	}

	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (in_buf = (unsigned char *)malloc(in_buf_size)));
	}
	if( ok ) {
		state = IS_MEM;
		ok = (NULL != (out_buf = (unsigned char *)malloc(out_buf_size)));
	}

	while( ok && IS_EOF != state ) {
		if( ok ) {
			state = IS_READ;
			rec = fread(in_buf, 1, in_buf_size, stdin);
			ok = (0 == ferror(stdin));
		}
		if( ok && in_buf_size != rec ) {
			state = IS_EOF;
			break;
		}
		if( ok ) {
			for(i = in_buf_size; i != -1; --i)
				in_buf[i] -= '0';
			if( doit&ENCODE )
				bch_encode(bch, in_buf, out_buf);
			else
				bch_decode(bch, in_buf, out_buf);
			for(i = out_buf_size; i != -1; --i)
				out_buf[i] += '0';
		}
		if( ok ) {
			state = IS_WRITE;
			fwrite(out_buf, out_buf_size, 1, stdout);
			ok = (0 == ferror(stdin));
		}
	}
	if( IS_EOF == state ) {
		state = IS_TAIL;
		ok = (0 == rec);
	}
	if( ok ) 
		delete_bch_code(bch);
	if( ok )
		goto __return_0;
		
	switch( state ) {
	case IS_MEM: fprintf(stderr, "%s: %s\n", argv_0, strerror(errno)); break;
	case IS_OPTION: fprintf(stderr, "%s: Указаны не все опции.\n", argv_0); break;
	case IS_READ: fprintf(stderr, "%s: read: %s\n", argv[0], strerror(errno)); break;
	case IS_WRITE: fprintf(stderr, "%s: write: %s\n", argv[0], strerror(errno)); break;
	case IS_TAIL: fprintf(stderr, "%s: Неожиданый конец входного потока (файл не выровнен).\n", argv_0); break;
	default: assert( 0 );
	}
	goto __return_1;
__return_0:
	free(in_buf);
	free(out_buf);
	return 0;
__return_1:
	free(in_buf);
	free(out_buf);
	return -1;
}
