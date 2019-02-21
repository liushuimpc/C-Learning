//
// Created by marco on 2/21/19.
//
#include <stdio.h>
#include <unistd.h>
#include <sys/stat.h>
#include <wait.h>
#include <asm/errno.h>
#include <errno.h>
#include <fcntl.h>

#include "fifo_sample.h"

static void do_sig(int signo)
{
    if (signo == SIGCHLD) {
        while (waitpid(-1, NULL, WNOHANG) > 0);
    }
}

int fifo_sample(void)
{
    int fdr, fdw;
    pid_t pid;

    char words[10] = "123456789";
    char buf[10] = {'\0'};

    if ((mkfifo(FIFO_PATH, 0777) == -1) && (errno != EEXIST) ) {
        _exit(-1);
    }
    printf("fifo: %s created successfully.\n", FIFO_PATH);

//    signal(SIGCHLD, do_sig);

    pid = fork();
    if (pid == 0) { // child
        printf("This is child\n");
        if ((fdw = open(FIFO_PATH, O_WRONLY)) < 0) {
            printf("Child cannot open %s, errno=%d\n", FIFO_PATH, errno);
            _exit(-1);
        }

        sleep(2);

        if (write(fdw, words, sizeof(words)) != sizeof(words)) {
            printf("Child cannot write %s\n", FIFO_PATH);
            _exit(-1);
        }

        printf("Child writed: %s\n", words);
        close(fdw);

    } else if (pid > 0) { // parent
        printf("This is Parent\n");
        if ((fdr = open(FIFO_PATH, O_RDONLY)) < 0) {
            printf("Parent cannot open %s, errno=%d\n", FIFO_PATH, errno);
            _exit(-1);
        }

        if (read(fdr, buf, sizeof(buf)) < 0) {
            printf("Parent cannot read %s\n", FIFO_PATH);
            _exit(-1);
        }

        printf("Parent read: %s\n", buf);
        close(fdr);
    }

    return 0;
}
