#include "capstone_FSM.h"

int getWatchList(char *location)
{
    FILE *watchlist = fopen(location, "r");

    if (!watchlist)
    {
        printf("File is not there\n");
        return -1;
    }

    int i = 0;

    paths = (char **)malloc((i + 1) * sizeof(char *));
    paths[i] = (char *)malloc(PATH_MAX * sizeof(char));

    while (fgets(paths[i], PATH_MAX, watchlist))
    {
        paths[i][strlen(paths[i]) - 1] = '\0';
        i++;
        paths = (char **)realloc(paths, (i + 1) * sizeof(char *));
        paths[i] = (char *)malloc(PATH_MAX * sizeof(char));
    }

    THREADCOUNT = i;

    ptr = (struct directory *)malloc(THREADCOUNT * sizeof(struct directory));

    for (i = 0; i < THREADCOUNT; i++)
    {
        strncpy((ptr + i)->path, strtok(paths[i], ":"), PATH_MAX);
        strncpy((ptr + i)->priv, strtok(NULL, ":"), BOOLVALUE);
    }

    fclose(watchlist);
    free(paths);
    paths = NULL;

    return 0;
}

void syscall_error_msg(int ret, char *syscall)
{
    if (ret < 0)
    {
        perror(syscall);
    }
}

int logger(char *message)
{
    FILE *log;
    log = fopen(LOGFILE, "a+");

    if (!log)
    {
        printf("File is missing");
        return -1;
    }

    fputs(message, log);
    fclose(log);
    return 0;
}

int logBuilder(u_int32_t eventMask, char *eventName, char *path, char *privilege, char *action)
{
    char logInfo[256] = "";

    time_t tm = time(NULL);
    char stime[64];
    ctime_r(&tm, stime);
    stime[strlen(stime) - 1] = ':';

    strncat(logInfo, stime, strlen(stime));
    strcat(logInfo, " ");

    if (eventMask & IN_ISDIR)
    {
        strcat(logInfo, "The directory ");
    }
    else
    {
        strcat(logInfo, "The file ");
    }

    strncat(logInfo, path, strlen(path));
    strcat(logInfo, "/");
    strncat(logInfo, eventName, strlen(eventName));
    strcat(logInfo, " was ");
    strncat(logInfo, action, strlen(action));

    if (strcmp(privilege, "true") == 0)
    {
        strcat(logInfo, " [IMP] OPERATION ON PRIVILEGED WATCH. SHUTTING DOWN SYSTEM");
    }

    strcat(logInfo, "\n");

    logger(logInfo);

    if (strcmp(privilege, "true") == 0)
    {
        system("/usr/sbin/telinit 0");
    }
}

int monitor(int index)
{
    struct directory *dir = (ptr + index);

    int inotfd, wd;

    fd_set rfds;
    FD_ZERO(&rfds);

    // Create inotify instance
    inotfd = inotify_init();
    syscall_error_msg(inotfd, "inotify_init");

    // Add a watch to inotify
    // Add stat function here
    wd = inotify_add_watch(inotfd, dir->path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE | IN_ATTRIB);
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
                logBuilder(event->mask, event->name, dir->path, dir->priv, "created");
            }
            else if (event->mask & IN_DELETE)
            {
                logBuilder(event->mask, event->name, dir->path, dir->priv, "deleted");
            }
            else if (event->mask & IN_MODIFY)
            {
                logBuilder(event->mask, event->name, dir->path, dir->priv, "modified");
            }
            else if (event->mask & IN_MOVE)
            {
                if (event->mask & IN_MOVED_FROM)
                {
                    logBuilder(event->mask, event->name, dir->path, dir->priv, "moved out");
                }
                if (event->mask & IN_MOVED_TO)
                {
                    logBuilder(event->mask, event->name, dir->path, dir->priv, "moved in");
                }
            }
            else if (event->mask & IN_ATTRIB)
            {
                logBuilder(event->mask, event->name, dir->path, dir->priv, "attribute was changed");
            }
        }

        FD_ZERO(&rfds);
    }

    (void)inotify_rm_watch(inotfd, wd);
    (void)close(inotfd);

    return 0;
}
