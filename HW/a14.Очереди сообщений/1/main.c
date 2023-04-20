#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

struct msbuf{
    long mtype; //- приоритет сообщения;
    char mtext[100]; //- само сообщение;
};

int main(void){

    key_t key_sms;
    //- для идентификатора очереди сообщения;
    int id_sms;
    //- структура, которую будем передавать, как сообщение
    struct msbuf msbuf;
    //- сообщение, которое будем передавать;
    char *send_dad = "Are you winning son?!";
    char *send_son = "Yes";
    //- название файла;
    char *name_file = "file_key.txt";
    //- для создания нового процесса;
    pid_t pid;
    //- для дескриптора созданного файла;
    int fd;
    //- для проверки завершения дочернего процесса;
    int status;
    ssize_t status_rcv;

    fd = open(name_file, O_CREAT | O_RDWR | O_TRUNC, S_IRUSR | S_IWUSR);

    key_sms = ftok(name_file, 6);    
        error_func(key_sms);

    pid = fork();
        error_func(pid);
    
    if(0 == pid){
        id_sms = msgget(key_sms, IPC_CREAT | 0666);
            error_func(id_sms);
        memset(msbuf.mtext, 0, sizeof(msbuf.mtext));
        status_rcv = msgrcv(id_sms, &msbuf, sizeof(msbuf) - sizeof(msbuf.mtype), 1, 0);
            error_func(status_rcv);
        printf("DAD: %s\n", msbuf.mtext);
        memset(msbuf.mtext, 0, sizeof(msbuf.mtext));
        msbuf.mtype = 2;
        snprintf(msbuf.mtext, sizeof(msbuf.mtext), "%s", send_son);
        status = msgsnd(id_sms, &msbuf, sizeof(msbuf) - sizeof(msbuf.mtype), 0);
            error_func(status);
    } else {
        id_sms = msgget(key_sms, IPC_CREAT | 0666);
            error_func(id_sms);      
        msbuf.mtype = 1;
        memset(msbuf.mtext, 0, sizeof(msbuf.mtext));
        snprintf(msbuf.mtext, sizeof(msbuf.mtext), "%s", send_dad);
        status = msgsnd(id_sms, &msbuf, sizeof(msbuf) - sizeof(msbuf.mtype), 0);
            error_func(status);
        
        pid = wait(&status);
            error_func(pid);
        if(!WIFEXITED(status)){
            printf("Дочерний процесс завершился некорректно");
        }

        memset(msbuf.mtext, 0, sizeof(msbuf.mtext));
        if(-1 == msgrcv(id_sms, &msbuf, sizeof(msbuf) - sizeof(msbuf.mtype), 2, 0)){
            printf("line:%d\n", __LINE__);
            perror("Неправильно приняли сообщение");
            exit(EXIT_FAILURE);
        }
        
        printf("SON: %s\n", msbuf.mtext);

        if(-1 == msgctl(id_sms, IPC_RMID, NULL)){
            perror("закрываем идентификатор потока сообщений");
            exit(EXIT_FAILURE);
        };

        if(-1 == close(fd)){
            perror("неправильно закрытие файла в родитиле");
            exit(EXIT_FAILURE);
        }
            error_func(status);

        if (-1 == remove("file_key.txt")) {
            perror("del file");
            return 1;
        }
    }
    exit(EXIT_SUCCESS);
}