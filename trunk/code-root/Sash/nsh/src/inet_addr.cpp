#include <sys/socket.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#include "client.h"

const unsigned short port_default = 1024;
		// порт поумолчанию для удалённого соединения

int set_inet_sockaddr(const char *str, struct sockaddr_in *sa) {
		// функция настраивающая адрес сокета по командной строке
	const unsigned int s_len = strlen(str);
	unsigned int i;
	char addr[s_len];
	unsigned long port;
	char *ptr;
	

	for(i = 0; i < s_len && ':' != str[i] ; ++i)
		addr[i] = str[i];
	addr[i] = 0;

	if( ':' == str[i] ) {
		port = strtoul(str + i + 1, &ptr, 0);
		if( ptr == NULL || *ptr != '\0' || port > 0xffff ) {
			h_errno = NO_RECOVERY;
			return -1;
		}
	} else 
		port = port_default;

	struct hostent *h_ent = gethostbyname(addr); 
	if( h_ent == NULL ) 
		return -1;
	
	memset(sa, 0, sizeof(*sa));
	
	sa->sin_family = h_ent->h_addrtype;
	memcpy(&sa->sin_addr, h_ent->h_addr, h_ent->h_length);
	
	sa->sin_port = htons(port);

	return 0;
}
