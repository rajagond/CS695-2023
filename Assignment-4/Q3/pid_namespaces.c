/* pid_namespaces.c

   Copyright 2023, Raja Gond
   Licensed under GNU General Public License v2 or later

   Demonstrate the operation of PID namespaces.
*/
#define _GNU_SOURCE
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sched.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>

/* A simple error-handling function: print an error message based
   on the value in 'errno' and terminate the calling process */

#define STACK_SIZE (1024 * 1024)    /* Stack size for cloned child */

#define errExit(msg)    do { perror(msg); exit(EXIT_FAILURE); \
                        } while (0)

static int              /* Start function for cloned child */
cloneFunc()
{

    printf("PID inside new PID namespace: %d \t Parent PID: %d\n", getpid(), getppid());
    
    char cwd[PATH_MAX];
    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("\nWorking Directory: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }
    /* change root directory*/
    char root_dir[PATH_MAX + sizeof("/root")];
    sprintf(root_dir, "%s/root", cwd);
    printf("Root Directory: %s\n", root_dir);
    if (chroot(root_dir) == -1)
        errExit("chroot");

    /* change working directory of this process*/
    if (chdir("/") == -1)
        errExit("chdir");

    if (getcwd(cwd, sizeof(cwd)) != NULL) {
        printf("Working Directory after chroot: %s\n", cwd);
    } else {
        perror("getcwd() error");
        return 1;
    }

    printf("\nRunning programs inside new PID namespace\n");
    
    /* fork 5 program p1.o p2.o p3.o p4.o p5.o */
    pid_t child_pid[5];
    for (int i = 0; i < 5; i++) {
        child_pid[i] = fork();
        if (child_pid[i] == -1)
            errExit("fork");
        if (child_pid[i] == 0) {
            // p1.o p2.o p3.o p4.o p5.o
            char prog[20];
            sprintf(prog, "./program/p%d.o", i + 1);
            char *argv[] = {prog, NULL};
            printf("Program Name: %s, PID: %d \t Parent PID %d\n", prog, getpid(), getppid());
            getcwd(cwd, sizeof(cwd));
            if (execv(argv[0], argv) == -1)
                errExit("execv");
        }
    }
    
    /* wait for all child process to terminate */
    for (int i = 0; i < 5; i++) {
        if (waitpid(child_pid[i], NULL, 0) == -1)      /* Wait for child */
            errExit("waitpid");
    }

    return 0;           /* Terminates child */
}


static char child_stack[STACK_SIZE];

int
main(int argc, char *argv[])
{
    pid_t child_pid;

    /* Create a child that has its own PID namespace;
       the child commences execution in cloneFunc() */
    pid_t parent_pid = getpid();

    child_pid = clone(cloneFunc, 
                    child_stack + STACK_SIZE,   /* Points to start of downwardly growing stack */ 
                    CLONE_NEWPID | SIGCHLD, NULL);
    
    if (child_pid == -1)
        errExit("clone");

    printf("PID of Container created by clone(): %d \t Main Program PID: %d\n", child_pid, parent_pid);

    /* Parent falls through to here */

    sleep(4);           /* Give some time to child */

    if (waitpid(child_pid, NULL, 0) == -1)      /* Wait for child */
        errExit("waitpid");

    printf("Terminating Cloned Process Or you can say mini container\n");

    exit(EXIT_SUCCESS);
}