#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "gost.h"

ssize_t readn(int fd, void *_buf, size_t _count) {
		char *buf = (char *)_buf;
		size_t count = _count;
		ssize_t rec;
		
		while( 0 != count ) {
				while( -1 == (rec = read(fd, buf, count)) && EINTR == errno );
				if( -1 == rec )
						return -1;
				else if( 0 == rec )
						return _count - count;
				count -= rec;
				buf += rec;
		}
		
		return _count;
}

ssize_t writen(int fd, const void *_buf, size_t _count) {
		char const *buf = (char const *)_buf;
		size_t count = _count;
		ssize_t rec;
		
		while( 0 != count ) {
				while( -1 == (rec = write(fd, buf, count)) && EINTR == errno );
				if( -1 == rec )
						return -1;
				count -= rec;
				buf += rec;
		}
		
		return _count;
}


GostKey_t gost_key = {
		'e', '5', 's', '6',
		'u', 'd', 'h', 'j',
		' ', ']', 'h', 'u',
		' ', 'd', 'i', 'g',
		'8', '4', 'u', 'f',
		' ', 'o', 'u', 'i',
		'7', '1', 'g', '1',
		'd', 'y', 'u', '4'
};

GostTable_t gost_table = {		
		0x03, 0x78, 0xeb, 0xbd,
		0x68, 0xc2, 0xbf, 0xcb,
		0x8b, 0xd5, 0x84, 0x3d,
		0x01, 0xf4, 0xdc, 0x08,
		0x4d, 0xbc, 0x5a, 0x28,
		0x23, 0x82, 0xf6, 0xca,
		0x70, 0xc0, 0xee, 0xcb,
		0x5f, 0x8b, 0xae, 0xba,
		0x0e, 0xfc, 0x93, 0xb3,
		0x19, 0xdd, 0xbd, 0x9c,
		0xcc, 0x6b, 0x5c, 0x5b,
		0x58, 0xd0, 0xb0, 0x8a,
		0xb4, 0x44, 0xbf, 0x3b,
		0x12, 0xf6, 0x75, 0xab,
		0xe1, 0x49, 0x86, 0x19,
		0xea, 0x76, 0x12, 0x45
};

GostSync_t gost_sync = {
		'0', '1', '2', '3',
		'4', '5', '6', '7'
};

int main(int argc, char **argv) {
	GostGamma_t code(gost_key, gost_table, gost_sync);
	
	u8_t buf[4096];

	while( true ) {
			size_t t1 = readn(0, buf, sizeof(buf));
			if( 0 == t1 )
					return 0;
			if( (size_t)-1 == t1 ) {
					fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
					return -1;
			}
			code.encode(buf, buf, t1);
			size_t t2 = writen(1, buf, t1);
			if( (size_t)-1 == t2 ) {
					fprintf(stderr, "%s: %s\n", argv[0], strerror(errno));
					return -1;
			}
	}
	
	return 0;
}
