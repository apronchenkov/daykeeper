#include "warnout.h"
#include "scrout.h"
#include "unistd.h"
#include "my_string.h"
#include "bool.h"

#define PREFIX_LENGTH 256
static char prefix[PREFIX_LENGTH];

void set_warning_prefix(const char *pref) {
	my_strncpy(prefix, pref, PREFIX_LENGTH);
}

void warn_out(const char *msg0, ...) {
	const char **mv = &msg0;
	int mc;
	
	bool ok = scrout(Err, prefix);
	
	for(mc = 0; ok && mv[mc]; ++mc)
		ok = scrout(Err, mv[mc]);
	
	if( !ok )
		exit(-1);
}
