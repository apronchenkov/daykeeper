#ifndef __sgnl_h__
#define __sgnl_h__

#include "warper.h"

void sgnl_set();
		// установить обработчики прерываний
void sgnl_unset();
		// снять обрабочики прерываний
bool sgnl_send(pid_t pid, unsigned int sgnl);
		// послать сигнал приложению
		// Если функция вернула значение false -> необходимо разорвать соединение приславшее данный пакет!
		
#endif //__sgnl_h__
