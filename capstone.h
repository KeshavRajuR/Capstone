#define _GNU_SOURCE
#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <inttypes.h>
#include <errno.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/inotify.h>
#include <sys/wait.h>
#include <linux/limits.h>


/* For backend.c */
#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN 1024

int dirCount;
char **paths;
struct timeval timeout;

#define LOGFILE "/media/aidynskullz/DATA/College_Dump/Sem6/capstone/monitor/events.log" // Move to /var/log/events.log


/* For thread.c */
#define THREADCOUNT dirCount
#define WATCHLISTLOCATION "/media/aidynskullz/DATA/College_Dump/Sem6/capstone/monitor/watchlist" // Move to /etc/watchlist


/* For main.c */
void *initialize();


/* From thread.c */
char **getWatchList(char *);
int monitor(char *);
void syscall_error_msg(int, char *);