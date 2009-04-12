#include "pool_list.h"
#include "pool.h"

#include "warper.h"
#include "my_errno.h"
#include "my_strerror.h"

#include "shellout.h"
#include "warnout.h"

#include "my_string.h"

static void pool_list_fd_write(char *str, int fd) {
	struct pollfd ufds;

	if( 0 >= fd ) {
		my_strcpy(str, "-");
		return;
	}

	ufds.fd = fd;
	ufds.events = POLLIN | POLLOUT;

	if( 0 > Poll(&ufds, 1, 0) ) {
		warn_out("list: Warning: Strange return from syscall poll(): ", my_strerror[errno], "\n", NULL);
		my_strcpy(str, "IOHE");
		return;
	}
	
	if( ufds.revents&POLLIN ) {
		*str = 'I';
		++str;
	}
	if( ufds.revents&POLLOUT ) {
		*str = '0';
		++str;
	}
	if( ufds.revents&POLLHUP ) {
		*str = 'H';
		++str;
	}
	if( ufds.revents&POLLERR ) {
		*str = 'E';
		++str;
	}

	*str = '\0';
}

void pool_list() {
		// Напечатать красивый список запущенных процессов
	struct io_t *ptr = pool;
	int i = 1;
	
	char no_buf[16];
	char gid_buf[16];
	char out_buf[5];
	char in_buf[5];
	char err_buf[5];
	
	shell_add_message("NO \t GID \t OUT \t IN \t ERR\n", NULL);
	
	while( NULL != ptr ) {
		my_l2str(no_buf, i);
		my_l2str(gid_buf, ptr->gid);

		pool_list_fd_write(out_buf, ptr->out);
		pool_list_fd_write(in_buf, ptr->in);
		pool_list_fd_write(err_buf, ptr->err);
		
		shell_add_message(no_buf, " \t ", gid_buf, " \t ", out_buf, " \t ", in_buf, " \t ", err_buf, "\n", NULL);

		++i;
		ptr = ptr->next;
	}
}
