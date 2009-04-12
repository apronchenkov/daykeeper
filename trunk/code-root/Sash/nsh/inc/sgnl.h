#ifndef __signal_h__
#define __signal_h__

extern unsigned int sgnl_count[];
		// счётчик сигналов 
extern const unsigned int sgnls;
		// число типов обрабытываемых сигналов
extern bool was_sgnl;
		// флаг указывающий на то, что приходил хотябы один сигнал

void sgnl_set();
		// установить обработчики прерываний
void sgnl_unset();
		// снять обрабочики прерываний
void sgnl_lock();
		// запретить прерывания
void sgnl_unlock();
		// разрешить прерывания

#endif //__signal_h__
