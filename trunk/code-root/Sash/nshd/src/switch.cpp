#include "switch.h"
#include "main.h"
#include "link.h"
#include "server.h"
#include "warper.h"
#include <sys/poll.h>

#include <errno.h>
#include <string.h>

bool switch_need_quit;

void switch_init() {
	// инициализация комутатор	
	server_listen_init();
	switch_need_quit = false;
}
void switch_halt() {
	// выключение комутатора
	server_listen_halt();
	switch_need_quit = true;
}

void switch_loop() {
	// главный цикл комутатора
	while( !switch_need_quit || 0 != links ) {
		struct pollfd ufds[4*links + 1];
		struct pollfd *ptr;
		link_t *link;

		link = links_begin();
		ptr = server_listen_request(ufds);
		while( NULL != link ) {
			ptr = server_link_request(link, ptr);
			link = links_next(link);
		}
			
		int rec;
		while( -1 == (rec = poll(ufds, ptr - ufds, -1)) && EINTR == errno );
		if( -1 == rec ) {
			warn_out("%s: poll: %s\n", argv_0, strerror(errno));
			continue;
		}
		
		link = links_begin();			// 1.
		server_listen_answer();			// 2. ВНИМАНИЕ! Порядок данных команд важен!
		while( NULL != link ) {
			server_link_answer(link);
			link = links_next(link);
		}
	}
}
