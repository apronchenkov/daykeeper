#ifndef __inet_addr_h__
#define __inet_addr_h__

#include <sys/types.h>
#include <sys/socket.h>

int set_inet_sockaddr(const char *str, struct sockaddr_in *_sa);
		// настроить структуру адреса сокета на основе входной строки

#endif //__inet_addr_h__
