#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <errno.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

int main()
{
    char *source, *text;
    int shmid, fd;
    char pathname[] = "2";

    key_t key;

    if ((key = ftok(pathname, 0)) < 0)
    {
        printf("Can't generate key\n");
        return -1;
    }

    // // 338716
    if ((fd = open("2a.c", O_RDONLY)) < 0)
    {
        printf("Can't open file\n");
        return -1;
    }

    lseek(fd, 0, SEEK_END);
    int size = lseek(fd, 0, SEEK_CUR);

    if ((shmid = shmget(key, size * sizeof(char), 0666 | IPC_CREAT | IPC_EXCL)) < 0)
    {
        if (errno != EEXIST)
        {
            printf("Can't create shared memory!\n");
            return -1;
        }

        if ((shmid = shmget(key, size * sizeof(char), NULL)) < 0)
        {
            printf("Can't find shared memory");
            return -1;
        }
    }

    source = (char *)shmat(shmid, NULL, 0);
    close(fd);

    if ((fd = open("2a.c", O_RDONLY)) < 0)
    {
        printf("Can't open file\n");
        return -1;
    }

    text = malloc(sizeof(char) * size);

    FILE *file = fopen("2a.c", "r");
    fread(text, sizeof(char), size, file);

    strncpy(source, text, size);

    if (shmdt(source) < 0)
    {
        printf("Can't detach shared memory\n");
        return -1;
    }

    return 0;
}