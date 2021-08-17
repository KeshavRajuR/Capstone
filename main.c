#include <stdio.h>
#include <unistd.h>
#include <inttypes.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/inotify.h>

#define EVENT_SIZE (sizeof(struct inotify_event))
#define BUF_LEN 1024
struct timeval timeout;

void syscall_error_msg(int ret, char *syscall)
{
    if (ret < 0)
    {
        perror(syscall);
    }
}

int main(int argc, char **argv)
{
    int inotfd, wd;

    fd_set rfds;
    FD_ZERO(&rfds);

    // Create inotify instance
    inotfd = inotify_init();

    syscall_error_msg(inotfd, "inotify_init");

    // Add a watch to inotify
    // Add stat function here
    const char *watch_path = "/home/aidynskullz/Desktop/Sem6/Useless/monitor";
    wd = inotify_add_watch(inotfd, watch_path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE);

    syscall_error_msg(wd, "inotify_add_watch");

    while (1)
    {
        FD_SET(inotfd, &rfds);
        timeout.tv_sec = 10;
        timeout.tv_usec = 0;

        char buffer[BUF_LEN];

        int res = select(FD_SETSIZE, &rfds, NULL, NULL, &timeout);

        syscall_error_msg(res, "select");

        int length = read(inotfd, &buffer, BUF_LEN);

        syscall_error_msg(length, "read");

        struct inotify_event *event = (struct inotify_event *)&buffer[0];

        if (event->len)
        {
            if (event->mask & IN_CREATE)
            {
                if (event->mask & IN_ISDIR)
                {
                    printf("The directory %s was created.\n", event->name);
                }
                else
                {
                    printf("The file %s was created.\n", event->name);
                }
            }
            else if (event->mask & IN_DELETE)
            {
                if (event->mask & IN_ISDIR)
                {
                    printf("The directory %s was deleted.\n", event->name);
                }
                else
                {
                    printf("The file %s was deleted.\n", event->name);
                }
            }
            else if (event->mask & IN_MODIFY)
            {
                if (event->mask & IN_ISDIR)
                {
                    printf("The directory %s was modified.\n", event->name);
                }
                else
                {
                    printf("The file %s was modified.\n", event->name);
                }
            }
            else if (event->mask & IN_MOVE)
            {               
                if (event->mask & IN_MOVED_FROM)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("The directory %s was moved out.\n", event->name);
                    }
                    else
                    {
                        printf("The file %s was moved out.\n", event->name);
                    }
                }
                if (event->mask & IN_MOVED_TO)
                {
                    if (event->mask & IN_ISDIR)
                    {
                        printf("The directory %s was moved in .\n", event->name);
                    }
                    else
                    {
                        printf("The file %s was moved in.\n", event->name);
                    }
                }
            }
        }

        FD_ZERO(&rfds);
    }

    (void)inotify_rm_watch(inotfd, wd);
    (void)close(inotfd);

    exit(0);
}
