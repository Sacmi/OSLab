#define _GNU_SOURCE

#include "fcntl.h"
#include "stdio.h"
#include "unistd.h"

int main()
{
    int fd[2];

    if (pipe(fd) < 0)
    {
        printf("Can\'t create pipe\n");
        return -1;
    }

    printf("Pipe size is %d\n", fcntl(fd[0], F_GETPIPE_SZ));
}