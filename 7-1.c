#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

int main()
{
    int fd, result;
    char name[] = "aaa.fifo";

    (void)umask(0);

    if (mkfifo(name, 0666) < 0)
    {
        printf("Can\'t create FIFO\n");
        exit(-1);
    }

    if ((fd = open(name, O_WRONLY)) < 0)
    {
        printf("Can\'t open FIFO for writting\n");
        exit(-1);
    }

    if (write(fd, "Hello, world!", 14) != 14)
    {
        printf("Can\'t write all string to FIFO\n");
        exit(-1);
    }

    close(fd);
    return 0;
}