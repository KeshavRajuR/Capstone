#include "capstone.h"

void *createThread(void *vargp)
{
    monitor((char *)vargp);
}

void *initialize()
{
    pthread_t tid;

    getWatchList(WATCHLISTLOCATION);

    for (int i = 0; i < THREADCOUNT; i++)
    {
        int thread = pthread_create(&tid, NULL, createThread, (void *)paths[i]);
        syscall_error_msg(thread, "pthread_create");
    }

    free(paths);
    paths = NULL;

    pthread_exit(NULL);

    pthread_join(tid, NULL);
}