#ifndef __keydata_h__
#define __keydata_h__

#include "gost.h"

class KeyData_t {
		GostKey_t gost_key;
		GostTable_t gost_table;
		GostSync_t gost_sync;
public:
		KeyData_t();
		~KeyData_t();

		const GostKey_t & get_gost_key() const;
		const GostTable_t & get_gost_table() const;
		const GostSync_t & get_gostsync() const;

};

#endif //__keydata_h__
