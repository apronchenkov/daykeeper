#include <malloc.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#define BUFFER_SIZE_STEP 1024*64
		// Параметр регулирующий, как быстро будет меняться размер буфера
#define MAX_FILE_LENGTH	(0xffffffff - BUFFER_SIZE_STEP)
		// искусственное ограничение на размер файла		
char *argv_0;
		// имя исполняемого файла (устанавливается из main()
int mix(int mx) {
	assert( 0 != mx );

	enum {INIT, IS_MEMORY, IS_READ, IS_FILE_LENGTH, IS_EOF, IS_FILE_LENGTH_MX, IS_WRITE} state = INIT;
	enum {false, true} ok = true;
	
	unsigned int buffer_size = BUFFER_SIZE_STEP;
			// размер буфера
	long long file_length = 0;
			// длинна файла
	char *buffer, *buffer2;
			// буфер
	const unsigned int m = (mx > 0 ? mx : -mx);
	
	if( ok ) {	
		state = IS_MEMORY;
		ok = (NULL != (buffer = malloc(buffer_size)));
	}
	
	while( ok && state != IS_EOF ) {
		int rec;
		if( ok ) {
			state = IS_READ;
			ok = ( 0 <= (rec = ReadN(STDIN, buffer + file_length, buffer_size - file_length)));
		}
		if( ok ) {
			state = IS_FILE_LENGTH;
			ok = ((file_length += rec) <= MAX_FILE_LENGTH);
		}
		if( ok ) {
			if( rec < buffer_size - file_length ) {
				state = IS_EOF;
				break;
			}
		}
		if( ok ) {
			state = IS_MEMORY;
			buffer_size += BUFFER_SIZE_STEP;
			ok = (NULL != (buffer = realloc(buffer, buffer_size)));
		}
	}
	
	if( ok ) {
		state = IS_FILE_LENGTH_MX;
		ok = (0 == (file_length%m));
	}
	
	if( ok ) {
		state = IS_MEMORY;
		ok = (NULL != (buffer2 = malloc(file_length)));
	}
	
	if( ok ) {
		//state = IS_MIX;
		unsigned int i, j, k = 0;
		if( mx > 0 ) 
			for(i = 0; i < m; ++i)
				for(j = i; j < file_length; j += m, ++k)
					buffer2[k] = buffer[j];
		else
			for(i = 0; i < m; ++i)
				for(j = i; j < file_length; j += m, ++k)
					buffer2[j] = buffer[k];
	}

	if( ok ) {
		state = IS_WRITE;
		ok = (file_length == WriteN(STDOUT, buffer2, file_length));
	}
	
	if( ok )
		return 0;
		
	switch( state ) {
	case IS_MEMORY: fprintf(stderr, "%s: memory: %s\n", argv_0, strerror(errno)); break;
	case IS_READ: fprintf(stderr, "%s: read: %s\n", argv_0, strerror(errno)); break;
	case IS_FILE_LENGTH: fprintf(stderr, "%s: Input string is too long.\n", argv_0); break;
	case IS_FILE_LENGTH_MX: fprintf(stderr, "%s: Input string is not multiple of [mix_block].\n", argv_0); break;
	case IS_WRITE: fprintf(stderr, "%s: write: %s\n", argv_0, strerror(errno)); break;
	default: assert( 0 );
	}
	
	return -1;
}

void usage() {
	fprintf(stderr, "%s: usage\n", argv_0);
	fprintf(stderr, "\t[-][mix_block]\n");
	fprintf(stderr, "\t[-] \n\t\tdemix input stream;\n");
	fprintf(stderr, "\t[mix_block] - number\n\t\tmixing input stream with [mix_block] size.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "Mixer (c) copyleft 2006 the.zett@gmail.com\n");
	exit(-1);
}

int main(int argc, char **argv) {
	argv_0 = argv[0];
	
	if( 1 == argc )
		usage();
		
	int mx;
	char *endptr;
	mx = strtol(argv[1], &endptr, 0);
	
	if( NULL == endptr || *endptr != '\0' || mx == 0 ) {
		fprintf(stderr, "[mix_block] value is not valid\n");
		return -1;
	}
	
	return mix(mx);
}
