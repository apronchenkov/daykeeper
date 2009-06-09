#ifndef __buffer_h__
#define __buffer_h__

#include "warper.h"

/* Описание пакетов: 
 * 1. Заголовок пакета
 * 	4 байта - длинная пакета (не считая заголовка)
 * 	1 байт	- тип пакета
 * 2. Типы пакетов
 * 	а) пакеты получаемые клиентом
 * 		1 - пакет для стандартного вывода
 * 		2 - пакет для потока ошибок
 * 	б) пакеты посылаемые клиентом
 * 		0 - пакеты из стандартного ввода ввода
 * 		3 - пакеты сигналов
 * 3. Пакеты сингналов
 * 	1 байта - номер сигнала
 * 	4 байта - сколько раз сигнал был получен
 * 	Пакет сигналов может описывать множество сигналов, при этом
 * 	размер пакета делится на пять (пять байт заголовок и по пять байт на каждый сигнал)
 * 
 * Замечания:
 * 	Из формата пакета сигналов видно, что наибольший номер сигнала 255.
 * 	Каждый получаемый пакет должен содержать как минимум заголовок
 */
#define PTYPE_SERVER_STDIN	0
#define PTYPE_CLIENT_STDOUT	1
#define PTYPE_CLIENT_STDERR	2
#define PTYPE_SERVER_SIGNAL	3 

#define PACKAGE_HEAD_SIZE	(5)
#define PACKAGE_DATA_SIZE 	(4096*4) 
#define SGNL_RECORD_SIZE	(5)
		// максимальный размер блока данных в пакете
		// должен быть достаточно большим, чтобы описывать все сигналы за раз
#define PACKAGE_SIZE (PACKAGE_DATA_SIZE + PACKAGE_HEAD_SIZE)
		// максимальным размер пакета

typedef enum{READY, ERROR, IOERROR, HUP, SEND, SEND_STATE1, RECIVE, RECIVE_STATE1, RECIVE_STATE2} io_state_t;
		// типы состояния буфера
/* Описание состояний буфера:
 * Общий случай: 
 * 	IOERROR - ошибка ввода вывода -> соединение должно быть разорвано;
 * Случай 1 (incoming) режим приёма пакета
 * 	READY - пакет получен;
 * 	ERROR - в заголовке указан не верный размер (слишком большой), либо пакет имеет не верный тип -> соединение должно быть разорвано;
 * 	RECIVE - буфер переключен в режим приёма, прежде чем читать данные необходимо сбросить
 * 		внутренние переменные буфера;
 * 	RECIVE_STATE1 - режим чтения заголовка пакета;
 * 	RECIVE_STATE2 - режим чтения тела пакета;
 * 	HUP - соединение разорвано.
 * 	В данном режиме все остальные состояния ошибочны.
 *
 * Случай 2 (outgoing_comp) режим передачи пакета
 * 	READY - пакет передан;
 * 	SEND - буфер переключен в режим передачи, прежде чем передавать данные необходимо сбросить
 * 	внутренние переменные;
 * 	SEND_STATE1 - режим передачи пакета.
 * 	В данном режиме все остальные состояния ошибочны.
 * 
 * Случай 3 (outgoing_data) режим передачи данных пакета (без заголовка)
 * 	READY - передача данных окончена;
 * 	SEND - (см. выше);
 * 	SEND_STATE1 - передача данных пакета.
 * 	В данном режиме все остальные состояния ошибочны.
 */

/*\/*/typedef struct sbuf {
	// структура буфера
	io_state_t state;
	unsigned int current;
		// текущая позиция в буфере
	union {
		char comp[PACKAGE_SIZE];
		// представление о пакета как вектора байт
		struct {
		// представление о пакете как о заголовке и данных
			char head[PACKAGE_HEAD_SIZE];
			char data[PACKAGE_DATA_SIZE];
		};
		struct {
		// то из чего состоит заголовок
			unsigned int size;
			unsigned char type;
		};
	};
} buf_t;
  
void incoming(buf_t &buf, int fd);
		// режим приёма пакета
void outgoing_data(buf_t &buf, int fd);
		// режим передачи пакета
void outgoing_comp(buf_t &buf, int fd);
		// режим передачи данных пакета (без заголовка)
		
#endif //__buffer_h__