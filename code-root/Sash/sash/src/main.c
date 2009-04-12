#include "pool.h"
#include "io.h"
#include "shell.h"

#include "my_string.h"
#include "warper.h"
#include "warnout.h"
#include "signl.h"

char **Envp;

int main(int argc, char **argv, char **envp) {
	char prefix[256];
	
	Setsid();
//	Setpgid(0, 0);
	
	Envp = envp;

	my_strncpy(prefix, argv[0], 253);
	my_strcat(prefix, ": ");
	
	set_warning_prefix(prefix);
	
	shll_halt = false;

	init_pool();
	
	signl_set();
	
	input();
	
	signl_remove();

	return 0;
}
