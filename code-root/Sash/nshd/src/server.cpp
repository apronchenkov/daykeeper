#include "server.h"
#include "switch.h"
#include "warper.h"
#include "sgnl.h"
#include "main.h"

#include <errno.h>
#include <string.h>
#include <assert.h>
#include <unistd.h>

static int listenfd = -1;
static struct pollfd *poll_listenfd;

static bool server_exec(int sockfd);
static bool server_send_sgnls(pid_t pid, buf_t &buf);

void server_listen_init() {
	assert( -1 == listenfd );
	enum{INIT, SOCK, SETOPT, SETCLEXEC, BIND, LISTEN} state = INIT;
	bool ok = true;
	int sockfd;
	int flag;
	struct sockaddr_in my_addr;
	
	if( ok ) {
		state = SOCK;
		ok = (-1 != (sockfd = Socket(AF_INET, SOCK_STREAM, 0)));
	}
	if( ok ) {
		state = SETOPT;
		flag = 1;
		ok = (-1 != Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &flag, sizeof(flag)));
	}
	if( ok ) {
		state = SETCLEXEC;
		ok = ( -1 != Fd_close_on_exec(sockfd));
	}
	if( ok ) {
		state = BIND;
		memset(&my_addr, 0, sizeof(my_addr));
		my_addr.sin_family = AF_INET;
		my_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		my_addr.sin_port = htons(serv_port);
		ok = (-1 != Bind(sockfd, (struct sockaddr*)&my_addr, sizeof(my_addr)));
	}
	if( ok ) {
		state = LISTEN;
		ok = (-1 != Listen(sockfd, 4));
	}
	if( ok ) {
		listenfd = sockfd;	
		warn_out("Interconnection is ready.\n");
		return;
	}
	
	switch( state ) {
	case SOCK: warn_out("%s: socket: %s\n", argv_0, strerror(errno)); break;
	case SETOPT: warn_out("%s: setsockopt: %s\n", argv_0, strerror(errno)); break;
	case SETCLEXEC: warn_out("%s: fd_close_on_exec: %s\b", argv_0, strerror(errno)); break;
	case BIND: warn_out("%s: bind: %s\n", argv_0, strerror(errno)); break;
	case LISTEN: warn_out("%s: listen: %s\n", argv_0, strerror(errno)); break;
	default: assert( 0 );
	}
	Exit(-1);
}

void server_listen_halt() {
	if( -1 == Close(listenfd) ) {
		warn_out("%s: close: %s\n", argv_0, strerror(errno));
//		Exit(-1);	
	}
	listenfd = -1;
}

struct pollfd * server_listen_request(struct pollfd *ufds) {
	if( listenfd < 0 )
		return ufds;

	poll_listenfd = ufds;
	poll_listenfd->fd = listenfd;
	poll_listenfd->events = POLLIN;

	return  ufds + 1;
}

void server_listen_answer() {	
	if( listenfd < 0 )
		return;
	assert( NULL != poll_listenfd );
	
	enum{INIT, IS_PERR, IS_PHUP, IS_PIN, ACC, CLEXEC, EXEC} state = INIT;
	bool ok = true;
	int fd;
	
	if( ok ) {
		state = IS_PERR;
		ok = !(poll_listenfd->revents&POLLERR);
	}
	if( ok ) {
		state = IS_PHUP;
		ok = !(poll_listenfd->revents&POLLHUP);
	}
	if( ok ) {
		state = IS_PIN;
		ok = (poll_listenfd->revents&POLLIN);
	}
	if( ok ) {
		state = ACC;
		struct sockaddr_in saddr;
		socklen_t addrlen;
		 fd = Accept(listenfd, (struct sockaddr*)&saddr, &addrlen);
		 ok = (fd >= 0);
	}
	if( ok ) {
		state = CLEXEC;
		ok = (0 == Fd_close_on_exec(fd));
	}
	if( ok ) {
		state = EXEC;
		ok = server_exec(fd);
	}
	
	if( ok || IS_PIN == state )
		return;

	switch( state ) {
	case IS_PERR: warn_out("%s: Warning: listen socket recive ERR -> server will be halt.\n", argv_0); switch_halt(); break;
	case IS_PHUP: warn_out("%s: Warning: listen socket recive HUP -> server will be halt.\n", argv_0); switch_halt(); break;
	case ACC: warn_out("%s: accept: %s\n", argv_0, strerror(errno)); break;
	case CLEXEC: 
		warn_out("%s: Fd_close_on_exec(fd): %s\n", argv_0, strerror(errno)); 
	case EXEC:
		if( -1 == Close(listenfd) ) warn_out("%s: close: %s\n", argv_0, strerror(errno));
		break;
	default: assert( 0 );
	}
}

void server_link_shut_connect(link_t *link) {
	if( -1 == Close(link->fd) ) 
		warn_out("%s: close(sockfd): %s\n", argv_0, strerror(errno));
	if( -1 != link->in && -1 == Close(link->in) )
		warn_out("%s: close(pipe_in): %s\n", argv_0, strerror(errno));
	if( -1 != link->out && -1 == Close(link->out) )
		warn_out("%s: close(pipe_out): %s\n", argv_0, strerror(errno));
	if( -1 != link->err && -1 == Close(link->err) )
		warn_out("%s: close(pipe_err): %s\n", argv_0, strerror(errno));
	link->fd = -1;
	link_delete(link);
}
void server_link_shut_in(link_t *link) {
	assert( link->exist && link->in >= 0 );
	if( -1 == Close(link->in) )
		warn_out("%s: close(pipe_in): %s\n", argv_0, strerror(errno));
	link->in = -1;
	++link->state;
	if( link->state == 3 )
		server_link_shut_connect(link);
}
void server_link_shut_out(link_t *link) {
	assert( link->exist && link->out >= 0 );
	if( -1 == Close(link->out) )
		warn_out("%s: close(pipe_out): %s\n", argv_0, strerror(errno));
	link->out = -1;
	++link->state;
	if( link->state == 3 )
		server_link_shut_connect(link);
}
void server_link_shut_err(link_t *link) {
	assert( link->exist && link->err >= 0 );
	if( -1 == Close(link->err) )
		warn_out("%s: close(pipe_err): %s\n", argv_0, strerror(errno));
	link->err = -1;
	++link->state;
	if( link->state == 3 )
		server_link_shut_connect(link);
}

static bool server_exec(int sockfd) {
	enum {INIT, PIPE_IN, PIPE_OUT, PIPE_ERR, FORK,
		CLOSE_PIPE_IN0, CLOSE_PIPE_OUT0, CLOSE_PIPE_ERR0, CLOSE_PIPE_IN1, CLOSE_PIPE_OUT1, CLOSE_PIPE_ERR1,
		DUP2_PIPE_IN, DUP2_PIPE_OUT, DUP2_PIPE_ERR, EXEC, SETCLEXEC_IN1, SETCLEXEC_OUT0, SETCLEXEC_ERR0} state = INIT;
		
	bool ok = true;
	
	int pipe_in[2], pipe_out[2], pipe_err[2];

	if( ok ) {
		state = PIPE_IN;
		ok = (0 == Pipe(pipe_in));
	}
	if( ok ) {
		state = PIPE_OUT;
		ok = (0 == Pipe(pipe_out));
	}
	if( ok ) {
		state = PIPE_ERR;
		ok = (0 == Pipe(pipe_err));
	}
	
	pid_t pid;
	
	if( ok ) {
		state = FORK;
		pid = Fork();
		ok = (-1 != pid);
	}
	
	if( 0 == pid ) {
		if( ok ) {
			state = CLOSE_PIPE_IN1;
			ok = (0 == Close(pipe_in[1]));
		}
		if( ok ) {
			state = CLOSE_PIPE_OUT0;
			ok = (0 == Close(pipe_out[0]));
		}
		if( ok ) {
			state = CLOSE_PIPE_ERR0;
			ok = (0 == Close(pipe_err[0]));
		}
		if( ok ) {
			state = DUP2_PIPE_IN;
			ok = (-1 != Dup2(pipe_in[0], 0));
		}
		if( ok ) {
			state = DUP2_PIPE_OUT;
			ok = (-1 != Dup2(pipe_out[1], 1));
		}
		if( ok ) {
			state = DUP2_PIPE_ERR;
			ok = (-1 != Dup2(pipe_err[1], 2));
		}
		if( ok ) {
			state = EXEC;
			ok = (-1 != execve(application, app_argv, app_envp));
		}
	} else {
		if( ok ) {
			state = CLOSE_PIPE_IN0;
			ok = (0 == Close(pipe_in[0]));
		}
		if( ok ) {
			state = CLOSE_PIPE_OUT1;
			ok = (0 == Close(pipe_out[1]));
		}
		if( ok ) {
			state = CLOSE_PIPE_ERR1;
			ok = (0 == Close(pipe_err[1]));
		}
		if( ok ) {
			state = SETCLEXEC_IN1;
			ok = (0 == Fd_close_on_exec(pipe_in[1]));
		}
		if( ok ) {
			state = SETCLEXEC_OUT0;
			ok = (0 == Fd_close_on_exec(pipe_out[0]));
		}
		if( ok ) {
			state = SETCLEXEC_ERR0;
			ok = (0 == Fd_close_on_exec(pipe_err[0]));
		}
		if( ok ) {
			link_new(sockfd, pipe_in[1], pipe_out[0], pipe_err[0], pid);
		}
	}
	
	if( ok )
		return true;
		
	switch( state ) {
		case EXEC:
				// это сообщение увидит клиент!!!
			warn_out("Warning: execve: %s\n", strerror(errno));
			Exit(-1);
			
				// сообщение представленные ниже будут напечатаны на стороне сервера
		case DUP2_PIPE_ERR:
		case DUP2_PIPE_OUT:
		case DUP2_PIPE_IN:
			warn_out("%s: child: dup2: %s\n", argv_0, strerror(errno));
			Exit(-1);
		case CLOSE_PIPE_ERR0:
		case CLOSE_PIPE_OUT0:
		case CLOSE_PIPE_IN1:
			warn_out("%s: child: close: %s\n", argv_0, strerror(errno));
			Exit(-1);
		
		case SETCLEXEC_ERR0:
		case SETCLEXEC_OUT0:
		case SETCLEXEC_IN1:
		case CLOSE_PIPE_ERR1:
		case CLOSE_PIPE_OUT1:
		case CLOSE_PIPE_IN0:
		case FORK:
			Close(pipe_err[0]);
			Close(pipe_err[1]);
		case PIPE_ERR:
			Close(pipe_out[0]);
			Close(pipe_out[1]);
		case PIPE_OUT:
			Close(pipe_in[0]);
			Close(pipe_in[1]);
		case PIPE_IN:
			warn_out("%s: There was error during application exec.\n", argv_0);
			return false;
		default:
			assert( 0 );
	}
}

static struct pollfd * server_link_incoming_request_send(link_t *link, struct pollfd *ufds) {
	assert( link->exist && link->incoming.type == PTYPE_SERVER_STDIN );
	if( link->in >= 0 ) 
		link->poll_in->events = POLLOUT;
	return ufds;
}

static struct pollfd * server_link_incoming_request_recive(link_t *link, struct pollfd *ufds) {
	assert( link->exist && link->fd >= 0 );
	link->poll_fd = ufds++;
	link->poll_fd->fd = link->fd;
	link->poll_fd->events = POLLIN;
	return ufds;
}

static struct pollfd * server_link_outgoing_request_ready(link_t *link, struct pollfd *ufds) {
	assert( link->exist );
	if( link->out >= 0 ) {
		link->poll_out = ufds++;
		link->poll_out->fd = link->out;
		link->poll_out->events = POLLIN;
	}
	if( link->err >= 0 ) {
		link->poll_err = ufds++;
		link->poll_err->fd = link->err;
		link->poll_err->events = POLLIN;
	}
	return ufds;
}

static struct pollfd * server_link_outgoing_request_send(link_t *link, struct pollfd *ufds) {
	assert( link->exist && link->fd >= 0 );
	if( NULL == link->poll_fd ) {
		link->poll_fd = ufds++;
		link->poll_fd->fd = link->fd;
		link->poll_fd->events = 0;
	}
	link->poll_fd->events |= POLLOUT;
	return ufds;		
}

struct pollfd * server_link_request(link_t *link, struct pollfd *ufds) {
	if( link->in >= 0 ) {
		link->poll_in = ufds++;
		link->poll_in->fd = link->in;
		link->poll_in->events = 0;
	}
	
	switch( link->incoming.state ) {
	case SEND:
	case SEND_STATE1:
		ufds = server_link_incoming_request_send(link, ufds);
		break;
	case RECIVE:
	case RECIVE_STATE1:
	case RECIVE_STATE2:
		ufds =  server_link_incoming_request_recive(link, ufds);
		break;
	default:
		assert( 0 );
	}

	switch( link->outgoing.state ) {
	case READY:
		ufds =  server_link_outgoing_request_ready(link, ufds);
		break;
	case SEND:
	case SEND_STATE1:
		ufds =  server_link_outgoing_request_send(link, ufds);
		break;
	default:
		assert( 0 );
	}
	return ufds;
}

static void server_link_incoming_answer_send(link_t *link) {
	assert( link->exist && link->in >= 0 && link->poll_in != NULL );
	
	enum{INIT, IS_PERR, IS_PHUP, IS_POUT, IS_IOERR, IS_READY} state = INIT;
	bool ok = true;

	if( ok ) {
		state = IS_PERR;
		ok = !(link->poll_in->revents&POLLERR);
	}
	if( ok ) {
		state = IS_PHUP;
		ok = !(link->poll_in->revents&POLLHUP);
	}
	if( ok ) {
		state = IS_POUT;
		ok = (link->poll_in->revents&POLLOUT);
	}
	if( ok ) {
		outgoing_data(link->incoming, link->in);
		state = IS_IOERR;
		ok = !(IOERROR == link->incoming.state);
	}
	if( ok ) {
		state = IS_READY;
		ok = (READY == link->incoming.state);
	}
	if( ok ) {
		link->incoming.state = RECIVE;
	}
	
	if( !ok && IS_POUT != state && IS_READY != state ) {
		switch( state ) {
		case IS_PERR:	warn_out("Warning: Application pipe_in recive ERR -> close in.\n"); break;
		case IS_PHUP:	warn_out("Warning: Application pipe_in recive HUP -> close in.\n"); break;
		case IS_IOERR: warn_out("Warning: Some thing gone wrong during send data to application -> close in.\n"); break;
		default: assert( 0 );
		}
		server_link_shut_in(link);
		link->incoming.state = RECIVE;
	}
}

static void server_link_incoming_answer_recive(link_t *link) {
	assert( link->exist && link->poll_fd != NULL );
	assert( link->in < 0 || link->poll_in != NULL );

	if( link->in >= 0 ) {
		// Часть первая - проверяем состояние link->in
		enum{INIT, IS_PERR, IS_PHUP} state = INIT;
		bool ok = true;
		if( ok ) {
			state = IS_PERR;
			ok = !(link->poll_in->revents&POLLERR);
		}
		if( ok ) {
			state = IS_PHUP;
			ok = !(link->poll_in->revents&POLLHUP);
		}
		
		if( !ok ) {
			switch( state ) {
			case IS_PERR:	warn_out("Warning: Application pipe_in recive ERR -> close in.\n"); break;
			case IS_PHUP:	warn_out("Warning: Application pipe_in recive HUP -> close in.\n"); break;
			default: assert( 0 );
			}
			server_link_shut_in(link);
		}
	}
	// Часть вторая  - проверяем состояние link->fd
	enum{INIT, IS_PERR, IS_PHUP, IS_PIN, IS_IOERR, IS_HUP, IS_ERR, IS_READY, IS_PACKSGNL} state = INIT;
	bool ok = true;
	
	if( ok ) {
		state = IS_PERR;
		ok = !(link->poll_fd->revents&POLLERR);
	}
	if( ok ) {
		state = IS_PHUP;
		ok = !(link->poll_fd->revents&POLLHUP) || (link->poll_fd->revents&POLLIN);
	}
	if( ok ) {
		state = IS_PIN;
		ok = (link->poll_fd->revents&POLLIN);
	}
	if( ok ) {
		incoming(link->incoming, link->fd);
		state = IS_IOERR;
		ok = !(IOERROR == link->incoming.state);
	}
	if( ok ) {
		state = IS_HUP;
		ok = !(HUP == link->incoming.state);
	}
	if( ok ) {
		state = IS_ERR;
		ok = !(ERROR == link->incoming.state);
	}
	if( ok ) {
		state = IS_READY;
		ok = (READY == link->incoming.state);
	}
	if( ok ) {
		if( link->incoming.type != PTYPE_SERVER_SIGNAL ) {
			if( link->in >= 0 )
				link->incoming.state = SEND;
			else
				link->incoming.state = RECIVE;
		} else {
			state = IS_PACKSGNL;
			ok = server_send_sgnls(link->pid, link->incoming);
			link->incoming.state = RECIVE;
		}
	}
	
	if( !ok && IS_PIN != state && IS_READY != state ) {
		switch( state ) {
		case IS_PERR: warn_out("Warning: Application socket_line recive ERR -> close link.\n"); break;
		case IS_PHUP: warn_out("Warning: Application socket_line recive HUP -> close link.\n"); break;
		case IS_IOERR: warn_out("Warning: Some thing gone wrong during transfer data throw socket -> close link.\n"); break;
		case IS_HUP: warn_out("Warning: Link was closed.\n"); break;
		case IS_ERR: warn_out("Warning: recived wrong package -> close link.\n"); break;
		case IS_PACKSGNL: warn_out("Warning: recived wrong package -> close link.\n"); break;
		default: assert( 0 );
		}
		server_link_shut_connect(link);
	}
}

static void server_link_outgoing_answer_ready(link_t *link) {
	if( !link->exist )
		return;
	assert( link->out >= 0 || link->poll_out == NULL );
	assert( link->err >= 0 || link->poll_err == NULL);
	
	enum{INIT, IS_PERR, IS_PHUP, IS_PIN, IS_IOERR, IS_HUP} state0 = INIT, state1 = INIT;
	bool ok0 = (link->out >= 0), ok1 = (link->err >= 0);
	
	if( ok0 ) {
		state0 = IS_PERR;
		ok0 = !(link->poll_out->revents&POLLERR);
	}
	if( ok0 ) {
		state0 = IS_PHUP;
		ok0 = !(link->poll_out->revents&POLLHUP) || (link->poll_out->revents&POLLIN);
	}
	if( ok0 ) {
		state0 = IS_PIN;
		ok0 = (link->poll_out->revents&POLLIN);
	}
	if( ok0 ) {
		int rec = Read(link->out, link->outgoing.data, PACKAGE_DATA_SIZE);
		if( ok0 ) {
			state0 = IS_IOERR;
			ok0 = (rec >= 0);
		}
		if( ok0 ) {
			state0 = IS_HUP;
			ok0 = (rec != 0);
		}
		if( ok0 ) {
			link->outgoing.size = rec;
			link->outgoing.type = PTYPE_CLIENT_STDOUT;
			link->outgoing.state = SEND;
		}
	}
	if( ok1 ) {
		state1 = IS_PERR;
		ok1 = !(link->poll_err->revents&POLLERR);
	}
	if( ok1 ) {
		state1 = IS_PHUP;
		ok1 = !(link->poll_err->revents&POLLHUP) || (link->poll_err->revents&POLLIN);
	}
	if( ok1 ) {
		state1 = IS_PIN;
		ok1 = (link->poll_err->revents&POLLIN) && !ok0;		// @@ Т.о. необходимо чтобы: имелась информация для чтения, и для link->out чтение не удалось.
	}
	if( ok1 ) {
		int rec = Read(link->err, link->outgoing.data, PACKAGE_DATA_SIZE);
		if( ok1 ) {
			state1 = IS_IOERR;
			ok1 = (rec >= 0);
		}
		if( ok1 ) {
			state1 = IS_HUP;
			ok1 = (rec != 0);
		}
		if( ok1 ) {
			link->outgoing.size = rec;
			link->outgoing.type = PTYPE_CLIENT_STDERR;
			link->outgoing.state = SEND;
		}
	}

	if( !ok0 && INIT != state0 && IS_PIN != state0 ) {
		switch( state0 ) {
		case IS_PERR: warn_out("Warning: Application pipe_out recive ERR -> close out.\n"); break;
		case IS_PHUP: warn_out("Warning: Application pipe_out recive HUP -> close out.\n"); break;
		case IS_IOERR: warn_out("Warning: Application pipe_out recive ERR -> close out.\n"); break;
		case IS_HUP: warn_out("Warning: Application pipe_out recive HUP -> close out.\n"); break;
		default: assert( 0 );
		}
		server_link_shut_out(link);
	}
	if( !ok1 && INIT != state1 && IS_PIN != state1 ) {
		switch( state1 ) {
		case IS_PERR: warn_out("Warning: Application pipe_err recive ERR -> close err.\n"); break;
		case IS_PHUP: warn_out("Warning: Application pipe_err recive HUP -> close err.\n"); break;
		case IS_IOERR: warn_out("Warning: Application pipe_err recive ERR -> close err.\n"); break;
		case IS_HUP: warn_out("Warning: Application pipe_err recive HUP -> close err.\n"); break;
		default: assert( 0 );
		}
		server_link_shut_err(link);
	}
}

static void server_link_outgoing_answer_send(link_t *link) {
	if( !link->exist )
		return;
	assert( link->fd >= 0 && link->poll_fd != NULL );
	enum{INIT, IS_PERR, IS_PHUP, IS_POUT, IS_IOERR} state = INIT;
	bool ok = true;

	if( ok ) {
		state = IS_PERR;
		ok = !(link->poll_fd->revents&POLLERR);
	}
	if( ok ) {
		state = IS_PHUP;
		ok = !(link->poll_fd->revents&POLLHUP);
	}
	if( ok ) {
		state = IS_POUT;
		ok = (link->poll_fd->revents&POLLOUT);
	}
	if( ok ) {
		outgoing_comp(link->outgoing, link->fd);
		state = IS_IOERR;
		ok = !(IOERROR == link->outgoing.state);
	}
	
	if( !ok && IS_POUT != state ) {
		switch( state ) {
		case IS_PERR:	warn_out("Warning: Connection recive ERR -> close link.\n"); break;
		case IS_PHUP:	warn_out("Warning: Connection recive HUP -> close link.\n"); break;
		case IS_IOERR: warn_out("Warning: Some thing gone wrong during send data throw socket -> close link.\n"); break;
		default: assert( 0 );
		}
				// server_link_shut_connect(link);
				// вообще-то говоря по идее надо было закрыть остальные соединения link->out и link->err
		server_link_shut_out(link);	// @@ Возможно нужна доп. теоретическая проработка!
		server_link_shut_err(link);
		link->outgoing.state = READY;
	}
}

void server_link_answer(link_t *link) {
	switch( link->incoming.state ) {
	case SEND:
	case SEND_STATE1:
		 server_link_incoming_answer_send(link);
		break;
	case RECIVE:
	case RECIVE_STATE1:
	case RECIVE_STATE2:
		server_link_incoming_answer_recive(link);
		break;
	default:
		assert( 0 );
	}
		
	switch( link->outgoing.state ) {
	case READY:
		server_link_outgoing_answer_ready(link);
		break;
	case SEND:
	case SEND_STATE1:
		server_link_outgoing_answer_send(link);
		break;
	default:
		assert( 0 );
	}
	link->poll_fd = NULL;
	link->poll_in = NULL;
	link->poll_out = NULL;
	link->poll_err = NULL;
}

static bool server_send_sgnls(pid_t pid, buf_t &buf) {
	assert( buf.type == PTYPE_SERVER_SIGNAL );
	assert( 0 == (buf.size%SGNL_RECORD_SIZE) );
	
	const unsigned int l = buf.size/SGNL_RECORD_SIZE;
	for(unsigned int i = 0; i < l; ++i) {
		const unsigned char sgnl = *(unsigned char*)(buf.data + i*SGNL_RECORD_SIZE);
		const unsigned int count = *(unsigned int*)(buf.data + i*SGNL_RECORD_SIZE + 1);
		for(unsigned int j = 0; j < count; ++j)
			if( false == sgnl_send(pid, sgnl) )
				return false;
	}

	return true;
}
