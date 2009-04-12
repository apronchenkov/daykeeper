#include "warper.h"
#include "errno.h"
#include "my_string.h"
#include "scrout.h"

bool scrout(int fd, const char *str) {
	unsigned int i, j;
	int rec;
	
	i = my_strlen(str);
	j = 0;
	
	do {
		if( rec < (rec = Write(fd, str + j, i - j)) )
			return false;
		j += rec;
	} while( j != i );

	return true;
}
