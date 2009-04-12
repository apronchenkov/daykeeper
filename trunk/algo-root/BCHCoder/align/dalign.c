#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <assert.h>

#define HEAD_SIZE	2
#define BODY_SIZE	1024*16

int dalign() {
	enum {INIT, IS_READ, IS_EOF, IS_WRITE, IS_END} state = INIT;
	enum {false, true} ok = true;

	unsigned short size;
	char data[BODY_SIZE];

	size_t rec;
	while( ok ) {
		if( ok ) {
			state = IS_READ;
			fread(&size, HEAD_SIZE, 1, stdin);
			ok = (0 == ferror(stdin));
		}
		if( ok ) {
			state = IS_END;
			ok = (0 != size);
		}
		if( ok ) {
			state = IS_EOF;
			ok = (0 == feof(stdin));
		}
		if( ok ) {
			state = IS_READ;
			rec = fread(data, size, 1, stdin);
			ok = (0 == ferror(stdin));
		}
		if( ok ) {
			state = IS_EOF;
			ok = (rec == 1);
		}
		if( ok ) {
			state = IS_WRITE;
			fwrite(data, size, 1, stdout);
			ok = (0 == ferror(stdout));
		}
		if( ok ) {
			state = IS_EOF;
			ok = (0 == feof(stdin));
		}
	}
	if( IS_END == state )
		return 0;
	switch( state ) {
	case IS_READ: fprintf(stderr, "align: read: %s\n", strerror(errno)); break;
	case IS_EOF: fprintf(stderr, "align: Unexpected end of file.\n"); break;
	case IS_WRITE: fprintf(stderr, "align: write: %s\n", strerror(errno)); break;
	default: assert( 0 );
	}
	return -1;
}
