#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define SHM_SIZE 200




int main(void){

    //- ключи для семафоров и для разделяемой памяти;
    key_t key;
    //- для проверки правильности исполнения функций;
    int status;
    char *sms_dad = "Are you winning son?";
    char *sms_son = "Yes!";
    char *bufer;
    //- идентификатор разделяемой памяти;
    int shmid;
    //- идентификатор семафора;
    int semid;
    //- идентификатор процесса
    pid_t pid;
    
    struct sembuf lock_1 = {0,-1,0};
    struct sembuf lock_2 = {0,0,0}; 

    key = ftok("key_memory", 47);
        error_func(key);
    
    shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666);
        error_func(shmid);

    semid = semget(key, 1, IPC_CREAT | 0666);
        error_func(semid);

    bufer = (char *)shmat(shmid, NULL, 0);

    status = semctl(semid, 0, SETVAL, 1); // установка начального значения семафора
        error_func(status);

    pid = fork();
        error_func(pid);

    if(0 == pid){
        //- блокируемся пока родитель не напишет сообщение
        semop(semid, &lock_2, 2);
        //- выводим, то что у нас для потомка;
        printf("DAD: %s\n", bufer);       

        memset(bufer,0,strlen(bufer));
        strcpy(bufer, sms_son);

        status = semctl(semid, 0, SETVAL, 1);
            error_func(status); 

    }else{
        //- родитель пишет сообщение, пока потомок заблокирован
        strcpy(bufer, sms_dad);

        status = semctl(semid, 0, SETVAL, 0);
            error_func(status);

        //- разблокируем общую память;
        semop(semid, &lock_1, 1);

        printf("SON: %s\n", bufer);

        pid = wait(&status);
            error_func(pid);
        if(!WIFEXITED(status)){
            printf("Дочерний процесс (1) завершился некорректно");
        }

        status = shmctl(shmid, IPC_RMID, NULL);
            error_func(status);

        status = semctl(semid, 0, IPC_RMID);
            error_func(status);
    }

    exit(EXIT_SUCCESS);
}

