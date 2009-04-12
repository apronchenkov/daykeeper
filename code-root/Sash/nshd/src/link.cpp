#include "link.h"

static link_t *link;
unsigned int links;

void link_new(int _fd, int _in, int _out, int _err, pid_t _pid) {
		// создать новый объект типа link_t
	link_t *l = new link_t;

	l->fd = _fd;
	l->in = _in;
	l->out = _out;
	l->err = _err;

	l->poll_fd = NULL;
	l->poll_in = NULL;
	l->poll_out = NULL;
	l->poll_err = NULL;

	l->pid = _pid;

	l->state = 0;
	l->incoming.state = RECIVE;
	l->outgoing.state = READY;

	l->exist = true;
	l->next = link;
	link = l;

	++links;
}


void link_delete(link_t *_l) {
	_l->exist = false;
	--links;
}

link_t * links_begin() {
	while( NULL != link && false == link->exist ) {
		link_t *l = link;
		link = l->next;
		delete l;
	}
	return link;
}

link_t * links_next(link_t *_l) {
	link_t *t;
	while( NULL != (t = _l->next) && false == t->exist ) {
		_l->next = t->next;
		delete t;
	}
	return _l->next;
}
