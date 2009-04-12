#include "unistd.h"
#include "my_string.h"
#include "my_errno.h"
#include "warnout.h"

extern char **Envp;

int my_exec(const char *filename, char *const argv[], char *const envp[]) {
	const int len = my_strlen(filename);
#define CMD_LENGTH 4096
	char cmd[CMD_LENGTH];

	char *path, *j;
	int i;
	
	if( 0 == my_strncmp("./", filename, sizeof("./") - 1) ) {
		return execve(filename, argv, envp);
	} else {
	
    		for(i = 0; 
    			Envp[i] && 
    			0 != my_strncmp(Envp[i], "PATH=", sizeof("PATH=") - 1);
    			++i);

    		if( NULL == Envp[i] ) {
    			warn_out("Variable PATH dos'not set.\n");
    			return ENOENT;
    		}

    		path = Envp[i] + sizeof("PATH=") - 1;

		if( '\0' == *path ) {
    			warn_out("Variable PATH is empty.\n");
    			return ENOENT;			
		}
	
    		do {
			for(j = path; '\0' != *j && ':' != *j; ++j);
	
			if( j - path + 1 + len > CMD_LENGTH ) {
	    			warn_out("Warning: Length of PATH + filename is too large.\n");
	    			continue;
			}
	
			my_strncpy(cmd, path, j - path);
			my_strcat(cmd, "/");
			my_strcat(cmd, filename);
			
			execve(cmd, argv, envp);
	
			if( ENOENT != errno ) 
				break;
				
	    		path = j + 1;

    		} while( '\0' != *j );
    
    		errno = ENOENT;
    		return -1;
	}
}
