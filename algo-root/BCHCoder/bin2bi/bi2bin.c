#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define BUFFER_SIZE (1024*4)

int main(int argc, char **argv) {
	enum{INIT, IS_READ, IS_WRITE, IS_EOF, IS_ALIGN} state = INIT;
	enum{false, true} ok = true;
	char in_b[BUFFER_SIZE];
	char out_b[BUFFER_SIZE/8];
	int len = 0;
	int shift = 0;
	
	int i, j;

	size_t rec;	
	while( ok && IS_EOF != state ) {
		if( ok ) 
			memcpy(in_b, in_b + shift, len - shift);
		if( ok ) {
			state = IS_READ;
			rec = fread(in_b + shift, 1, BUFFER_SIZE - shift, stdin);
			ok = (0 == ferror(stdin));
		}
		if( ok ) {
			len = rec + shift;
			shift = len - len%8;


			for(i = 0; i < shift/8; ++i)
				for(j = 0; j < 8; ++j) 
					switch( in_b[i*8 + j] ) {
					case '0': out_b[i] <<= 1; break;
					case '1': out_b[i] <<= 1; out_b[i] ^= 1; break;
					default: assert( 0 );
					}
		}
		if( ok ) {
			state = IS_WRITE;
			fwrite(out_b, shift/8, 1, stdout);
			ok = (0 == ferror(stdout));
		}
		if( ok ) {
			state = IS_EOF;
			ok = (0 == feof(stdin));
		}
	}
	if( IS_EOF == state ) {
		state = IS_ALIGN;
		ok = (len == shift);
	}
	if( ok )
		return 0;
	switch( state ) {
	case IS_READ: fprintf(stderr, "%s: read: %s\n", argv[0], strerror(errno)); break;
	case IS_WRITE: fprintf(stderr, "%s: write: %s\n", argv[0], strerror(errno)); break;
	case IS_ALIGN: fprintf(stderr, "%s: Length of input is not multipy of 8.\n", argv[0]); break;
	default: assert( 0 );
	}
	return -1;
}
