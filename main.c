#include "capstone.h"

static volatile sig_atomic_t done = 0;
static int deathfd = -1;

static void handle_done(int signum)
{
    if (!done)
    {
        done = signum;
    }
}

static int install_done(const int signum)
{
    struct sigaction act;

    memset(&act, 0, sizeof act);
    sigemptyset(&act.sa_mask);
    act.sa_handler = handle_done;
    act.sa_flags = 0;
    if (sigaction(signum, &act, NULL) == -1)
    {
        return errno;
    }
    return 0;
}

static void death(int signum, siginfo_t *info, void *context)
{
    if (info->si_code == POLL_IN && info->si_fd == deathfd)
    {
        raise(SIGTERM);
    }
}

static int install_death(const int signum)
{
    struct sigaction act;

    memset(&act, 0, sizeof act);
    sigemptyset(&act.sa_mask);
    act.sa_sigaction = death;
    act.sa_flags = SA_SIGINFO;
    if (sigaction(signum, &act, NULL) == -1)
    {
        return errno;
    }
    return 0;
}

int main()
{
    pid_t child, p;
    int pipefd[2], status;
    char buffer[8];

    if (install_done(SIGINT))
    {
        fprintf(stderr, "Cannot set SIGINT handler: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* Construct Pipe */
    if (pipe(pipefd) == -1)
    {
        fprintf(stderr, "Cannot create control pipe: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

    /* Fork child process */
    child = fork();
    if (child == (pid_t)-1)
    {
        fprintf(stderr, "Cannot fork child process: %s.\n", strerror(errno));
        return EXIT_FAILURE;
    }

/* Child process: */
CHILD:
    if (child == 0)
    {
        /*
            * Child process.
            */

        /* Close write end of pipe. */
        close(pipefd[1]);
        deathfd = pipefd[0];

        /* Set a SIGHUP signal handler. */
        if (install_death(SIGHUP))
        {
            fprintf(stderr, "Child process: cannot set SIGHUP handler: %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        /* Set SIGTERM signal handler. */
        if (install_done(SIGTERM))
        {
            fprintf(stderr, "Child process: cannot set SIGTERM handler: %s.\n", strerror(errno));
            return EXIT_FAILURE;
        }

        /*
            * Set read end of pipe to generate pipe signal (F_SETSIG)
            * We want a SIGHUP instead of SIGIO.
            */
        fcntl(deathfd, F_SETSIG, SIGHUP);

        /* 
            * Set own PID as read end owner
            * We want the SIGHUP delivered when deathfd closes.
            */
        fcntl(deathfd, F_SETOWN, getpid());

        /* 
            * Set read end of pipe nonblocking and async
            * Make the deathfd (read end of pipe) nonblocking and async.
            */
        fcntl(deathfd, F_SETFL, O_NONBLOCK | O_ASYNC);

        /* 
            * Check if the parent process is dead.
            * This is when parent is dead before child can start its execution
            */
        if (read(deathfd, buffer, sizeof buffer) == 0)
        {
            printf("Child process (%ld): Parent process is already dead.\n", (long)getpid());
            fflush(stdout);
            return EXIT_FAILURE;
        }

        /* Actual code to be executed by the child */

        pthread_t thread;
        int monitorThread = pthread_create(&thread, NULL, initialize, NULL);

        while (1)
        {
            status = __atomic_fetch_and(&done, 0, __ATOMIC_SEQ_CST);
            if (status == SIGINT)
            {
                printf("Child process (%ld): SIGINT caught and ignored.\n", (long)getpid());
            }
            else if (status)
            {
                break;
            }
            printf("Child process (%ld): Tock.\n", (long)getpid());
            fflush(stdout);
            sleep(1);
        }

        printf("Child process (%ld): Exited due to %s.\n", (long)getpid(),
               (status == SIGINT) ? "SIGINT" : (status == SIGHUP) ? "SIGHUP"
                                           : (status == SIGTERM)  ? "SIGTERM"
                                                                  : "Unknown signal.\n");
        fflush(stdout);

        return EXIT_SUCCESS;
    }

    /*
     * Parent process.
    */

    /* Close read end of pipe. */
    close(pipefd[0]);

    while (!done)
    {
        fprintf(stderr, "Parent process (%ld): Tick.\n", (long)getpid());
        fflush(stderr);
        sleep(1);

        /* Try reaping the child process. */
        p = waitpid(child, &status, WNOHANG);
        if (p == child || (p == (pid_t)-1 && errno == ECHILD))
        {
            if (p == child && WIFSIGNALED(status))
            {
                fprintf(stderr, "Child process died from %s. Parent will now spawn another child process.\n",
                        (WTERMSIG(status) == SIGINT) ? "SIGINT" : (WTERMSIG(status) == SIGHUP) ? "SIGHUP"
                                                              : (WTERMSIG(status) == SIGTERM)  ? "SIGTERM"
                                                                                               : "an unknown signal");
            }
            else
            {
                fprintf(stderr, "Child process has exited, Parent will restart another child process.\n");
            }
            fflush(stderr);
            child = fork();
            goto CHILD;
        }
    }

    if (done)
    {
        fprintf(stderr, "Parent process (%ld): Exited due to %s, will proceed to kill child process before exiting.\n", (long)getpid(),
                (done == SIGINT) ? "SIGINT" : (done == SIGHUP) ? "SIGHUP"
                                                               : "Unknown signal.\n");
        fflush(stderr);

        /* Kill child process */
        kill(child, SIGTERM);
    }

    /* Never reached! */
    return EXIT_SUCCESS;
}