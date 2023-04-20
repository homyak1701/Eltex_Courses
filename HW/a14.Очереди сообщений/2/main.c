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
#include <mqueue.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    mqd_t mqt;
    struct mq_attr attr;
    //- буфер для принятия сообщения;
    char buf_sms[1024];
    //- для создания нового процесса;
    pid_t pid;
    //- для проверки завершения дочернего процесса;
    int status;
    ssize_t status_rcv;
    //- возвращает приоритет;
    unsigned int prio;

    //- создание атрибутов очереди
    attr.mq_maxmsg = 10; //- максимальное количество сообщений в очереди;
    attr.mq_msgsize = 100; //- максимальный размер сообщения;

    mqt = mq_open("/my_mq", O_CREAT | O_RDWR, 0666, &attr);
        error_func(mqt);

    pid = fork();
        error_func(pid);
    
    if(0 == pid){

        memset(buf_sms, 0, sizeof(buf_sms));
        
        status_rcv = mq_receive(mqt, buf_sms, attr.mq_msgsize, &prio);
            error_func(status_rcv);

        printf("DAD: %s\n", buf_sms);

        //- отправляет сообщение с высшим приоритетом, чтобы 
        //родительский процесс прочитал второе сообщение
        status = mq_send(mqt, "Yes", strlen("Yes") + 1, 5);
            error_func(status);

        status = mq_close(mqt);
            error_func(status);

    } else if(0 < pid){

        memset(buf_sms, 0, sizeof(buf_sms));

        status = mq_send(mqt, "Are you winning son?!", strlen("Are you winning son?!") + 1, 1);
            error_func(status);
        
        pid = wait(&status);
            error_func(pid);
        if(!WIFEXITED(status)){
            printf("Дочерний процесс завершился некорректно");
        }

        memset(buf_sms, 0, sizeof(buf_sms));

        if(-1 == mq_receive(mqt, buf_sms, attr.mq_msgsize, &prio)){
            printf("line:%d\n", __LINE__);
            perror("Неправильно приняли сообщение");
            exit(EXIT_FAILURE);
        }
        
        printf("SON: %s\n", buf_sms);

        status = mq_close(mqt);
            error_func(status);

    } else if(0 > pid){
        perror("Ошибка при создания процесса");
        exit(EXIT_FAILURE);
    }
    exit(EXIT_SUCCESS);
}