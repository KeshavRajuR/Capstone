#include "capstone_FSM.h"

void *createThread(void *index)
{
    monitor((intptr_t)index);
}

void *initialize()
{
    pthread_t tid;

    getWatchList(WATCHLISTLOCATION);

    for (int i = 0; i < THREADCOUNT; i++)
    {
        int thread = pthread_create(&tid, NULL, createThread, (void *)(intptr_t)i);
        syscall_error_msg(thread, "pthread_create");
    }

    pthread_exit(NULL);

    pthread_join(tid, NULL);

    free(ptr);
}
