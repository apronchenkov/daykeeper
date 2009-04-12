#include "inet_addr.h"
#include "warper.h"
#include "client.h"
#include "sgnl.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>

void usage() {
		// функция печатаюцая подсказку
	fprintf(stderr, "usage: \n");
	fprintf(stderr, "\t hostname:port\n");
	fprintf(stderr, "(c) 2006 copyleft the.zett@gmail.com\n");
	exit(-1);
}

int main(int argc, char **argv) {
		// выдаём подсказку 
	if( argc < 2 )
		usage();
	
		// настраиваем адресс
	struct sockaddr_in sa;
	if( -1 == set_inet_sockaddr(argv[1], &sa) ) {
		fprintf(stderr, "%s: %s\n", argv[0], hstrerror(h_errno));
		exit(-1);
	}
	
		// создаём сокет
	int sockfd;
	if( -1 == (sockfd = Socket(sa.sin_family, SOCK_STREAM, 0)) ) {
		fprintf(stderr, "%s: socket: %s\n", argv[0], strerror(errno));
		exit(-1);
	}
		// создаём соединение
	if( 0 != Connect(sockfd, (struct sockaddr*)&sa, sizeof(sa)) ) {
		fprintf(stderr, "%s: connect: %s\n", argv[0], strerror(errno));
		exit(-1);
	}
		// инициализируем данные клиента		
	client_init(sockfd);
		// устанавливаем обработчик сингалов
	sgnl_set();
		// запускаем главную функцию клиента
	client_switching();
		// убираем наши обработчики сигналов
	sgnl_unset();

		// закрываем соединение
	if( -1 == Close(sockfd) ) {
		fprintf(stderr, "%s: close: %s\n", argv[0], strerror(errno));
		exit(-1);
	}
	
	return 0;
}
