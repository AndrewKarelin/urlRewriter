#ifndef __RELOAD_H_
#define __RELOAD_H_

#ifdef __cplusplus
extern "C" {
#endif

/*signal USR1 handler - reloading redirect list*/
void my_handler(int signum);

/* start inotify watcher and register signal handler for SIGUSR1 */
void init_reloaders(void);

#ifdef __cplusplus
}
#endif

#endif /* __RELOAD_H_ */
