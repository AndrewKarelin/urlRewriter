#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <unistd.h>

#include "dict.h"
#include "reload.h"
#include "tools.h"

#define MAX_URL_LEN 2048

volatile int ready = 0;
volatile nlist ** dict = NULL;
char * filename;

int main(int argc, char *argv[])
{
	openlog(argv[0], LOG_PID | LOG_NDELAY, LOG_USER);
	setlogmask(LOG_UPTO(LOG_INFO));

	filename = parse_args(argc, argv);

	syslog(LOG_INFO, "Start with redirection list %s ", filename);

	init_reloaders();

	dict = (volatile nlist **) load_dict(filename);

	ready = 1;

	char buf[MAX_URL_LEN]; // input from squid

	setbuf(stdout, NULL); // for work without fflush

	syslog(LOG_DEBUG, "Ready. Wait for url in stdin");

	while (fgets(buf, MAX_URL_LEN, stdin) != NULL)
	{
		char *p;

		if ((p = strchr(buf, '\n')) != NULL)
			*p = '\0'; /* strip \n */

		syslog(LOG_DEBUG, "Got %s from squid", buf);

		while (!ready)
			usleep(100); //wait until redirect dictionary be ready

		ready = 0; // redirect dictionary is busy
		nlist *np = lookup(buf, (nlist **) dict);
		ready = 1; // redirect dictionary is not busy

		if (np == NULL) //not found
			printf("ERR\n");  // send 'no-change' result back to Squid
		else
			printf("OK rewrite-url=%s\n", np->defn); // send new url result back to Squid
	}

	syslog(LOG_INFO, "stdin closed, exiting");
	closelog();
	return 0;
}
