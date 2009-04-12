#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#include "align.h"
#include "dalign.h"

char *argv_0;

void usage() {
	fprintf(stderr, "%s: usage\n", argv_0);
	fprintf(stderr, "\t[-d | align]\n");
	fprintf(stderr, "\t[-d] \n\t\tdisalign input stream;\n");
	fprintf(stderr, "\t[align] - number\n\t\talign input stream with [align] cell size.\n");
	fprintf(stderr, "\n");
	fprintf(stderr, "(c) copyleft 2006 the.zett@gmail.com\n");
	exit(-1);
}

int main(int argc, char **argv) {
	argv_0 = argv[0];
	
	if( 1 == argc )
		usage();
		
	if( 0 == strcmp(argv[1], "-d" ) )
		return dalign();
	
	unsigned int alg;
	char *endptr;
	alg = strtoul(argv[1], &endptr, 0);
	
	if( NULL == endptr || *endptr != '\0' || alg == 0 ) {
		fprintf(stderr, "[align] value is not valid\n");
		return -1;
	}
	
	return align(alg);
}
