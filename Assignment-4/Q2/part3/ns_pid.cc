#include <sys/types.h>
#include <sys/syscall.h>
#include <unistd.h>
#include <poll.h>
#include <stdlib.h>
#include <stdio.h>
#include <sched.h>

#ifndef __NR_pidfd_open
#define __NR_pidfd_open 434   /* System call # on most architectures */
#endif

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

static int
pidfd_open(pid_t pid, unsigned int flags)
{
    return syscall(__NR_pidfd_open, pid, flags);
}

int
main(int argc, char *argv[])
{
    int pidfd;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <pid> <program>\n", argv[0]);
        errExit("usage error");
    }

    pidfd = pidfd_open(atol(argv[1]), 0);
    if (pidfd == -1) {
        errExit("pidfd_open");
    }

    if(setns(pidfd, CLONE_NEWUTS) == -1)         /* Join that namespace */
    {
        errExit("setns");
    }
    close(pidfd);                      /* Now we can close the file descriptor */
    execvp(argv[2], &argv[2]);      /* Execute a command in namespace */
    perror("execvp");               /* execvp() only returns on error */
    exit(EXIT_SUCCESS);
}