#ifndef __server_h__
#define __server_h__

#include <sys/poll.h>

#include "link.h"

void server_listen_init();
		// инициализация сокета прослушивания
void server_listen_halt();
		// закрытие сокета прослушивания

struct pollfd * server_listen_request(struct pollfd *ufds);
		// построение запроса к poll от сокета прослушивания
void server_listen_answer();
		// принятие ответа от poll сокетом прослушивания

void server_link_shut_connect(link_t *link);
		// закрытие соединения с клиентом

struct pollfd * server_link_request(link_t *link, struct pollfd *ufds);
		// построение запроса к poll от соединения
void server_link_answer(link_t *link);
		// принятие ответа от poll соединением

#endif //__server_dosomething_h__
