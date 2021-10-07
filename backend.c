#include "capstone.h"

char **getWatchList(char *location)
{
    FILE *watchlist = fopen(location, "r");

    if (!watchlist)
    {
        printf("File is not there\n");
        return paths;
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

    fclose(watchlist);

    return paths;
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

int logBuilder(u_int32_t eventMask, char *eventName, char *path, char *action)
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
    strcat(logInfo, "\n");

    logger(logInfo);
}

int monitor(char *pathToWatch)
{
    int inotfd, wd;

    fd_set rfds;
    FD_ZERO(&rfds);

    // Create inotify instance
    inotfd = inotify_init();

    syscall_error_msg(inotfd, "inotify_init");

    // Add a watch to inotify
    // Add stat function here
    const char *watch_path = pathToWatch;
    wd = inotify_add_watch(inotfd, watch_path, IN_CREATE | IN_DELETE | IN_MODIFY | IN_MOVE | IN_ATTRIB);
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
                logBuilder(event->mask, event->name, pathToWatch, "created");
            }
            else if (event->mask & IN_DELETE)
            {
                logBuilder(event->mask, event->name, pathToWatch, "deleted");
            }
            else if (event->mask & IN_MODIFY)
            {
                logBuilder(event->mask, event->name, pathToWatch, "modified");
            }
            else if (event->mask & IN_MOVE)
            {
                if (event->mask & IN_MOVED_FROM)
                {
                    logBuilder(event->mask, event->name, pathToWatch, "moved out");
                }
                if (event->mask & IN_MOVED_TO)
                {
                    logBuilder(event->mask, event->name, pathToWatch, "moved in");
                }
            }
            else if (event->mask & IN_ATTRIB)
            {
                logBuilder(event->mask, event->name, pathToWatch, "attribute was changed");
            }
        }

        FD_ZERO(&rfds);
    }

    (void)inotify_rm_watch(inotfd, wd);
    (void)close(inotfd);

    return 0;
}