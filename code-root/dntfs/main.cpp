#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "partition.h"
#include "mft.h"
#include "mft_record.h"
#include "interface.h"

char const *argv_0;

class partition_t *partition;
class mft_t *mft;
class mft_record_t const *mft_record;
class attribute_t const *attribute;

void usage() {
	fprintf(stderr, "%s: Использование.\n", argv_0);
	exit(-1);
}

int main(int const argc, char const **argv) {

	argv_0 = argv[0];

	if( 1 == argc ) {
		usage();
	}

	FILE *file = fopen(argv[1], "r");
	if( NULL == file ) {
		fprintf(stderr, "%s: открытие(\"%s\"): %s\n", argv_0, argv[1], strerror(errno));
		exit(-1);
	}
	
	try {
		partition = new partition_t(file);
		
		mft = new mft_t;

		char BUF[partition->get_bytes_per_mft_record()];
		mft->read_mft_record_data(0, BUF);

		cli();

		if( NULL != mft_record )
			delete mft_record;

		if( NULL != mft )
			delete mft;
		
		if( NULL != partition )
			delete partition;
	} catch(bool) {}

	return 0;
}
