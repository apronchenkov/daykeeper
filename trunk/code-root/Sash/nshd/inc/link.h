#ifndef __link_h__
#define __link_h__

#include "buffer.h"
#include "warper.h"

typedef struct slink_t {
		// структура описывающая соединение
		// замечание, данная структура не является тождественной соединению
		// т.е. объект данного типа может существовать независимо от реального соединения
		// и наоборот.
	bool exist;
		// флаг существования соединения
	unsigned char state;
		// вспомогательный счётчик (число закрытых pipeов данного соединения. Если 3 - значит связь разрушена полностью -> надо закрывать линию.
	int fd;
		// сетевой дискриптор	
	int in, out, err;
		// дискрипторы работы с приложением
	struct pollfd *poll_fd;
	struct pollfd *poll_in, *poll_out, *poll_err;
		// поля для доступа к poll ответам
	pid_t pid;
		// номер приложения
	buf_t incoming;
		// буфер для приходящих пакетов
	buf_t outgoing;
		// буфер для исходящих пакетов
	struct slink_t *next;
} link_t;

extern unsigned int links;

void link_new(int _fd, int _in, int _out, int _err, pid_t _pid);
void link_delete(link_t *_l);
link_t * links_begin();
link_t * links_next(link_t *_l);

#endif //__link_h__
