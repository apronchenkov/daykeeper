#ifndef __main_h__
#define __main_h__

#include "partition.h"
#include "mft.h"
#include "mft_record.h"
#include "attribute.h"

extern char const *argv_0;

extern class partition_t *partition;
extern class mft_t *mft;
extern class mft_record_t const *mft_record;
extern class attribute_t const *attribute;

#endif //__main_h__
