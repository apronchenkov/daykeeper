#include "buffer.h"
#include "warper.h"

#include <assert.h>

void incoming(buf_t &buf, int fd) {
		// режим приёма пакета (описание состояний см. в заголовочном файле)
	int ret;
	
	switch( buf.state ) {
	case RECIVE:
		buf.state = RECIVE_STATE1;
		buf.current = 0;

	case RECIVE_STATE1:
		ret = Read(fd, buf.head + buf.current, PACKAGE_HEAD_SIZE - buf.current);
		if( ret < 0 )
			return;
		if( ret == 0 ) {
			buf.state = HUP;
			return;
		}

		buf.current += ret;
	
		if( buf.current < sizeof(unsigned int) ) 
			break;

		if( buf.size > PACKAGE_DATA_SIZE ) {
			buf.state = ERROR;
			warn_out("Warning! Resived package has too big size. May be some one want to hack you.\n");
			warn_out("May be in some future it will be drop this package, but now it -> hard halt.\n");
			Exit(-1);
		}
		buf.state = RECIVE_STATE2;
		buf.current = 0;
		break;
		
	case RECIVE_STATE2:
		ret = Read(fd, buf.data + buf.current, buf.size - buf.current);
		if( ret < 0 )
			return;
		if( ret == 0 ) {
			buf.state = HUP;
			return;
		}
		buf.current += ret;
	
		if( buf.current < buf.size  )
			break;
		buf.state = READY;
		break;

	default:
		assert( 0 );
	}

	return;
}

void outgoing_comp(buf_t &buf, int fd) {
	int ret;
	
	switch( buf.state ) {
	case SEND:
		buf.state = SEND_STATE1;
		buf.current = 0;

	case SEND_STATE1:
		ret = Write(fd, buf.comp + buf.current, buf.size + PACKAGE_HEAD_SIZE - buf.current);
		if( ret <= 0 )
			return;
		buf.current += ret;
		
		if( buf.current < buf.size + PACKAGE_HEAD_SIZE )
			break;
		buf.state = READY;
		break;

	default:
		assert( 0 );
	}
}

void outgoing_data(buf_t &buf, int fd) {
	int ret;
	
	switch( buf.state ) {
		
	case SEND:
		buf.state = SEND_STATE1;
		buf.current = 0;

	case SEND_STATE1:
		ret = Write(fd, buf.data + buf.current, buf.size - buf.current);
		if( ret <= 0 )
			return;
		buf.current += ret;
		
		if( buf.current < buf.size )
			break;
		buf.state = READY;
		break;

	default:
		assert( 0 );
	}	
}
