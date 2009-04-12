#ifndef __client_h__
#define __client_h__

#include "buffer.h"

extern link_t ilink;
		// структура описывающая соединение
void client_init(int fd);
		// инициализация клиента
void client_switching();
		// запуск системы обработки пакетов; главный цикл программы

#endif //__client_h__
