#define _GNU_SOURCE // F_SETSIG
#include <signal.h>
#include <stdio.h>
#include <string.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <fcntl.h>
#include <syslog.h>
#include <bits/stdint-uintn.h>
#include <errno.h>
#include <string.h>


#include "reload.h"
#include "dict.h"
#include "tools.h"


extern volatile int ready;
extern volatile nlist ** dict;
extern char * filename;
int fd;

#define EVENT_SIZE  ( sizeof (struct inotify_event) )
#define EVENT_BUF_LEN     ( 1024 * ( EVENT_SIZE + 16 ) )

void init_reloaders(void)
{
	struct sigaction act;
	memset(&act, 0, sizeof(act));

	// Restart the system call, if at all possible
	act.sa_flags = SA_RESTART;
	act.sa_handler = my_handler;

	// Block every signal during the handler
	sigfillset(&act.sa_mask);

	if (sigaction(SIGUSR1, &act, NULL) != 0)
		syslog(LOG_ERR,"sigaction(SIGUSR1,...:%s",strerror(errno));
	else
		syslog(LOG_INFO,
				"Handler for SIGUSR1 working (reload redirection list)");

	fd = inotify_init();
	if (fd < 0)
	{
		syslog(LOG_ERR,"inotify_init: %s",strerror(errno));
		return;
	}

	int fl = fcntl(fd, F_GETFL);
	fl |= O_ASYNC | O_NONBLOCK; /* want a signal on fd ready */
	fcntl(fd, F_SETFL, fl);
	fcntl(fd, F_SETSIG, SIGUSR1);
	fcntl(fd, F_SETOWN, getpid()); /* send it to our pid */

	uint32_t mask = ( IN_MODIFY | IN_ATTRIB | IN_CLOSE_WRITE | IN_CREATE
			| IN_DELETE_SELF | IN_MOVE_SELF);

	/*add watch and checking for error*/
	if (inotify_add_watch(fd, filename, mask) < 0)
	{
		syslog(LOG_ERR,"inotify_add_watch: %s",strerror(errno));
		close(fd);
		return;
	}
	syslog(LOG_INFO,
			"Inotify watch for file %s working (reload redirection list)",
			filename);

}

void my_handler(int signum)
{
	if (signum != SIGUSR1)
		return;

	while (!ready)
		usleep(100); //wait until redirect dictionary be not busy

	ready = 0; // redirect dictionary is busy

	free_dict((nlist **) dict);

	dict = (volatile nlist **) load_dict(filename);

	ready = 1; // redirect dictionary is not busy

	syslog(LOG_INFO, "Redirection list reloaded");

	char buf[EVENT_BUF_LEN];

	int numread = read(fd, buf, EVENT_BUF_LEN);

	if (numread > 0)
		syslog(LOG_DEBUG, "Inotify event %d bytes ", numread);
}
