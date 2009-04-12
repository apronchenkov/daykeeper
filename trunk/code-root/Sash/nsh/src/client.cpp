#include <sys/types.h>
#include <sys/poll.h>

#include <errno.h>
#include <string.h>

#include <assert.h>

#include "client.h"
#include "warper.h"
#include "sgnl.h"

#define STDIN	0
#define STDOUT	1
#define STDERR	2

link_t ilink;
		// структура описывающая соединение
static bool need_quit;
		// флаг выхода; если истина - пора выходить

/* Описание основного цикла:
 * 	Система разбивается на три части:
 * 	центральная систем,  система обработки прходящих сообщений,
 * 	система создания и отправки исходящих пакетов.
 * 	
 * 	1. Центральная система: далает запрос к другим системам - в результате 
 * 	формируется запрос к операционной системе (системный вызов poll). После
 * 	зароса, соответствующие результаты отправляются к системам.
 * 
 */
static void client_incoming_pollfd(struct pollfd &ufds);
		// функция построения запроса приходящего буфера
static void client_outgoing_pollfd(struct pollfd &ufds);
		// функция построения запроса исходящего буфера
static void client_incoming_switching(struct pollfd &ufds);
		// функция обработки ответа приходящим буфером
static void client_outgoing_switching(struct pollfd &ufds);
		// функция обработки ответа исходящим буфером

void client_init(int fd) {
		// фукнция инициализации клиента
	need_quit = false;
	ilink.fd = fd;
	ilink.incoming.state = RECIVE;
	ilink.outgoing.state = READY;
}

void client_switching() {
		// центральная система
	struct pollfd ufds[2];
	while( !need_quit ) {
		client_incoming_pollfd(ufds[0]);
		client_outgoing_pollfd(ufds[1]);
		
		int ret = poll(ufds, 2, -1);

		if( -1 == ret && EINTR == errno )
			continue;
		if( -1 == ret ) {
			warn_out("Warning: poll: %s -> Shell will halt.\n", strerror(errno));
			need_quit = true;
		}

		client_incoming_switching(ufds[0]);
		client_outgoing_switching(ufds[1]);
	}
}

static void client_incoming_pollfd(struct pollfd &ufds) {

	switch( ilink.incoming.state ) {
	case SEND:
	case SEND_STATE1:
		ufds.fd = ilink.incoming.type;
		ufds.events = POLLOUT;
		break;
			
	case RECIVE:
	case RECIVE_STATE1:
	case RECIVE_STATE2:
		ufds.fd = ilink.fd;
		ufds.events = POLLIN;
		break;
			
	default:
		assert( 0 );
	}
}

static void client_incoming_switching(struct pollfd &ufds) {
	
	switch( ilink.incoming.state ) {
		
	case SEND:
	case SEND_STATE1:
		if( ufds.revents&POLLERR ) 
			if( ilink.incoming.type == PTYPE_CLIENT_STDERR ) 
				Exit(-1);
			else {
				warn_out("Warning: Stdout has error state -> client will be halt.\n");
				need_quit = true;
				break;
			}
		if( ufds.revents&POLLHUP ) 
			if( ilink.incoming.type == PTYPE_CLIENT_STDERR ) 
				Exit(-1);
			else {
				warn_out("Warning: Stdout has HUP state -> client will be halt.\n");
				need_quit = true;
				break;
			}
		if( !(ufds.revents&POLLOUT) )
			break;

		// вызов обработчика
		outgoing_data(ilink.incoming, ilink.incoming.type);
		
		if( READY == ilink.incoming.state ) 
			ilink.incoming.state = RECIVE;
		break;
	
	case RECIVE:
	case RECIVE_STATE1:
	case RECIVE_STATE2:
		if( ufds.revents&POLLERR ) {
			warn_out("Warning: ILINK has error state -> client will be halt.\n");
			need_quit = true;
			break;
		}
		if( ufds.revents&POLLHUP ) {
			warn_out("Warning: ILINK hase HUP state -> client will be halt.\n");
			need_quit = true;
			break;
		}
		if( !(ufds.revents&POLLIN) )
			break;

		// вызов обработчика
		incoming(ilink.incoming, ilink.fd);

		if( ERROR == ilink.incoming.state ) {
			warn_out("Warning: Recived package with wrong size field -> halt.\n");
			need_quit = true;
			break;
		}
		if( HUP == ilink.incoming.state ) {
			warn_out("Warning: Connection was close by server -> halt.\n");
			need_quit = true;
			break;
		}
	
		if( READY != ilink.incoming.state )
			break;
		if( ilink.incoming.type == PTYPE_CLIENT_STDOUT ||
		    ilink.incoming.type == PTYPE_CLIENT_STDERR ) {
		    	ilink.incoming.state = SEND;
		} else {
			warn_out("Warning: Was recived package of wrong type -> client will be hard halt!\n");
			need_quit = true;
		}
		break;
	default:
		assert(0);
	}
}

static void client_outgoing_pollfd(struct pollfd &ufds) {
	switch( ilink.outgoing.state ) {
	case READY:
		if( !was_sgnl ) {
			ufds.fd = STDIN;
			ufds.events = POLLIN;
			break;
			
		} else {
			unsigned int i;
			int j;
			sgnl_lock();		// @@
			was_sgnl = false;
			for(i = j = 0; i < sgnls; ++i)
				if( sgnl_count[i] > 0 ) {
					*(unsigned char*)(ilink.outgoing.data + j) = i;
					j += 1;
					*(unsigned int *)(ilink.outgoing.data + j) = sgnl_count[i];
					j += 4;
					sgnl_count[i] = 0;
				}
			sgnl_unlock();		//@@
			ilink.outgoing.type = PTYPE_SERVER_SIGNAL;
			ilink.outgoing.size = j;
			ilink.outgoing.state = SEND;
		}

	case SEND:
	case SEND_STATE1:
		ufds.fd = ilink.fd;
		ufds.events = POLLOUT;
		break;
		
	default:
	assert(0);
	}
}

static void client_outgoing_switching(struct pollfd &ufds) {

	switch( ilink.outgoing.state ) {

	case SEND:
	case SEND_STATE1:
		if( ufds.revents&POLLERR ) {
			warn_out("Warning: ILINK has error state -> client will be halt.\n");
			need_quit = true;
			break;
		}
		if( ufds.revents&POLLHUP ) {
			warn_out("Warning: ILINK hase HUP state -> client will be halt.\n");
			need_quit = true;
			break;
		}
		// вызов обработчика
		outgoing_comp(ilink.outgoing, ilink.fd);
		break;
	
	case READY:
		if( ufds.revents == 0 || was_sgnl )
			break;
		ilink.outgoing.state = RECIVE;
		// к этому переходу нужно относиться очень аккуратно
	case RECIVE:
		if( ufds.revents&POLLERR ) {
			warn_out("Warning: Stdin has error state -> client will be halt.\n");
			need_quit = true;
			break;
		} 
		if( ufds.revents&POLLHUP ) {
			warn_out("Warning: Stdin has HUP state -> client will be halt.\n");
			need_quit = true;
			break;
		}
		if( ufds.revents&POLLIN ) {
			int ret = Read(STDIN, ilink.outgoing.data, PACKAGE_DATA_SIZE);
			if( ret < 0 ) {
				warn_out("Warning: Stdin has error state -> client will be halt.\n");
				need_quit = true;
				break;
			}
			if( ret == 0 ) {
				warn_out("Warning: Stdin has HUP state -> client will be halt.\n");
				need_quit = true;
				break;
			}
			ilink.outgoing.type = PTYPE_SERVER_STDIN;
			ilink.outgoing.size = ret;
			ilink.outgoing.state = SEND;
		}
		break;
	
	default:	
		assert(0);
	}
}

