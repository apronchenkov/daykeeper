#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "warper.h"
#include "switch.h"

unsigned short serv_port = 1025;

char *application;
char **app_argv;
char **app_envp;

char *argv_0;

int main(int argc, char **argv, char **envp) {
	printf("Netshell(sash) (c) 2006 the.zett@gmail.com\n");
	if( argc < 2 ) 
		Exit(0);
	argv_0 = argv[0];	
			// создаём среду запуска приложений
	application = argv[1];
	app_envp = envp;
	
	char *app_argv2[argc];
	for(int i = 0; i < argc - 1; ++i)
		app_argv2[i] = argv[i + 1];
	app_argv2[argc - 1] = NULL;
	
	app_argv = app_argv2;
			// запускаем комутатор
	switch_init();
	switch_loop();
	
	return 0;
}
