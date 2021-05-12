/* Программа, осуществляющая однонаправленную связь через pipe между процессом-родителем и процессом-ребенком */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sem.h>

int main()
{
    int fd[2], result;
    size_t size;

    char resstring[14];

    int semid; /* IPC дескриптор для массива IPC семафоров */

    char pathname[] = "task4"; /* Имя файла, использующееся для генерации ключа. Файл с таким именем должен существовать в текущей директории */

    key_t key; /* IPC ключ */

    struct sembuf mybuf; /* Структура для задания операции над семафором */

    if ((key = ftok(pathname, 0)) < 0) {
        printf("Can\'t generate key\n");
        exit(-1);
    }

    if ((semid = semget(key, 1, 0666 | IPC_CREAT)) < 0) {
        printf("Can\'t get semid\n");
        exit(-1);
    }

    if (pipe(fd) < 0)
    {
        printf("Can\'t create pipe\n");
        exit(-1);
    }

    mybuf.sem_flg = 0;
    mybuf.sem_num = 0;

    result = fork();

    if (result < 0)
    {
        printf("Can\'t fork child\n");
        exit(-1);
    } else if (result > 0) {
        size = write(fd[1], "Hello, world!", 14);

        if (size != 14)
        {
            printf("Can\'t write all string\n");
            exit(-1);
        }

        mybuf.sem_op = -1;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        close(fd[1]);

        // чтение

        size = read(fd[0], resstring, 14);

        if (size < 0)
        {
            printf("Can\'t read string\n");
            exit(-1);
        }

        printf("%s\n", resstring);
        close(fd[0]);
        printf("Parent exit\n");

    } else
    {
        size = read(fd[0], resstring, 14);

        if (size < 0)
        {
            printf("Can\'t read string\n");
            exit(-1);
        }

        printf("%s\n", resstring);
        close(fd[0]);

        mybuf.sem_op = 1;

        if(semop(semid, &mybuf, 1) < 0){
            printf("Can\'t wait for condition\n");
            exit(-1);
        }

        // запись

        size = write(fd[1], "Hello, world!", 14);

        if (size != 14)
        {
            printf("Can\'t write all string\n");
            exit(-1);
        }

        close(fd[1]);
    }

    return 0;
}