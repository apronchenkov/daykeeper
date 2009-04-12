/* Модуль Align
 * 
 * Входные параметры: 
 * 1. Параметр выравнивания
 * 2. Исходная строка
 * 
 * Результат:
 * Модифицырованная исходная строка
 * 
 * Модификация заглючается в следующем: 
 * 1. В начало приписывается длинна исходной строки в байтах
 * 2. Строка полученная на после приписывания дополняется в
 * конце нулями, т.о. чтобы длинна результата была кратна параметру 
 * выравниванияи 8 (восьми).
 * 
 * Длинная файла - 4 байта (возникает естественное ограничение на файлы длинны > 2^32 - 1
 */

#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#define HEAD_SIZE	2
#define BODY_SIZE	1024*16

unsigned short size;
char data[BODY_SIZE];
		// буфер	

unsigned int gcd(unsigned int a, unsigned int b) {
gcd_loop:
	if( a == 0 )
		return b;
	b %= a;
	if( b == 0 )
		return a;
	a %= b;
	goto gcd_loop;
}
unsigned int lcm(unsigned int a, unsigned int b) {
	return a/gcd(a, b)*b;
}

int align(unsigned int align) {
	enum {INIT, IS_READ, IS_EOF, IS_WRITE} state = INIT;
	enum {false, true} ok = true;
	
	unsigned long long file_length = 0;
	unsigned long long tail_length;
	unsigned long long l_align;

	size_t rec;
	while( ok ) {

		if( ok ) {
			state = IS_READ;
			rec = fread(data, 1, BODY_SIZE, stdin);
			ok = (0 == ferror(stdin));
		}
		if( ok ) {
			state = IS_WRITE;
			size = rec;
			file_length += HEAD_SIZE;
			fwrite(&size, HEAD_SIZE, 1, stdout);
			ok = (0 == ferror(stdout));
		}
		if( ok ) {
			state = IS_WRITE;
			file_length += rec;
			fwrite(data, rec, 1, stdout);
			ok = (0 == ferror(stdout));
		}
		if( ok ) {
			state = IS_EOF;
			ok = (0 == feof(stdin));
		}
	}
	if( state = IS_EOF ) 
		ok = true;
	if( ok ) {
		state = IS_WRITE;
		size = 0;
		fwrite(&size, HEAD_SIZE, 1, stdout);
		ok = (0 == ferror(stdout));
	}
	if( ok ) {
		l_align = lcm(8, align);
		file_length += HEAD_SIZE;
		tail_length = (l_align - file_length%l_align)%l_align;
	}
	if( ok )
		memset(data, 0, BODY_SIZE);
	while( ok && tail_length >= BODY_SIZE ) {
		state = IS_WRITE;
		fwrite(data, 1, BODY_SIZE, stdout);
		ok = (0 == ferror(stdout));
		tail_length -= BODY_SIZE;
	}
	if( ok ) {
		state = IS_WRITE;
		fwrite(data, 1, tail_length, stdout);
		ok = (0 == ferror(stdout));
	}
	if( ok )
		return 0;
		
	switch( state ) {
	case IS_READ: fprintf(stderr, "align: read: %s\n", strerror(errno)); break;
	case IS_WRITE: fprintf(stderr, "align: write: %s\n", strerror(errno)); break;
	default: assert( 0 );
	}
	
	return -1;
}
