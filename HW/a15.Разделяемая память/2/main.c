#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <semaphore.h>
#include <string.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define NAME_SHM "/name_shm"
#define NAME_SEM_S "/name_sem_son"
#define NAME_SEM_P "/name_sem_par"

int main(void){

    //- идентифиактор разделяемой памяти;
    int shm_fd;
    //- для отслеживания работы фукнции;
    int status;
    //- буфер для хранения сообщений;
    char *bufer = NULL;
    //- чтобы отличать родительский и дочерний процессы
    pid_t pid;
    //- идентификатор семафора POSIX
    sem_t *sem_p = sem_open(NAME_SEM_P, O_CREAT, 0666, 0);
    sem_t *sem_ch = sem_open(NAME_SEM_S, O_CREAT, 0666, 0);
    //- сообщения процессов
    char *sms_dad = "Are you winning son?";
    char *sms_son = "Yes!";

    //- получаем дескриптор разделямой памяти;
    shm_fd = shm_open(NAME_SHM, O_CREAT | O_RDWR, 0666);
        error_func(shm_fd);
    //- зададим размер разделяемой памяти;
    status = ftruncate(shm_fd, 100);
        error_func(status);
    //- отобразим выделенную память в наше пространство;
    bufer = (char *)mmap(NULL, 100, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);

    pid = fork();
        error_func(pid);
    
    if(0 == pid){

        status = sem_wait(sem_p);
            error_func(status);

        printf("DAD: %s\n", bufer);       

        memset(bufer,0,strlen(bufer));
        strcpy(bufer, sms_son); 
        
        status = sem_post(sem_ch);

        status = sem_close(sem_p);
            error_func(status);
        
        status = sem_close(sem_ch);
            error_func(status);

    } else {

        strcpy(bufer, sms_dad);

        status = sem_post(sem_p);
            error_func(status);

        status = sem_wait(sem_ch);
            error_func(status);

        printf("SON: %s\n", bufer);

        pid = wait(&status);
            error_func(pid);
        if(!WIFEXITED(status)){
            printf("Дочерний процесс завершился некорректно");
        }

        status = sem_close(sem_p);
            error_func(status);

        status = munmap(bufer, strlen(bufer));
            error_func(status);
        
        status = sem_unlink(NAME_SEM_P);
            error_func(status);

        status = sem_unlink(NAME_SEM_S);
            error_func(status);

        status = shm_unlink(NAME_SHM);
            error_func(status);
    }

    exit(EXIT_SUCCESS);
}