#include "scrout.h"
#include "warnout.h"
#include "shellout.h"
#include "my_string.h"
#include "my_strerror.h"
#include "unistd.h"
#include "errno.h"
#include "bool.h"

#define MESSAGES	512
#define MESSAGE_LENGTH	512

static char messages[MESSAGES][MESSAGE_LENGTH];
static unsigned int messages_count;

void shell_add_message(const char *msg0, ...) {
	const char **mv = &msg0;
	unsigned int mc;
	
	for(mc = 0; mv[mc] && messages_count < MESSAGES ; ++mc, ++messages_count)
		my_strncpy(messages[messages_count], mv[mc], MESSAGE_LENGTH);
	
	if( mv[mc] ) 
		warn_out("Warning: Too many messages to print to shell!\n", NULL);
}

void shell_out() {
	unsigned int i;
	bool ok = true;
	
	for(i = 0; ok && i < messages_count; ++i)
		ok = scrout(Out, messages[i]);
	
	if( ok )
		ok = scrout(Out, "# ");
	
	if( !ok ) {
		warn_out("Fatal: output error: ", my_strerror[errno], "\n", NULL);
		exit(-1);
	}

	messages_count = 0;
}
