#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define MAIN_PORT 3451

int main(void){

    struct sockaddr_in serv;
    struct sockaddr_in client;
    //- тут храним размер для фукнции accept();
    socklen_t len = sizeof(client);
    //- дескриптор сокета;
    int fd_serv;
    int fd_request;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    int num;
    char num_char[10];

    fd_serv = socket(AF_INET, SOCK_DGRAM, 0);
        error_func(fd_serv);

    //-описывам наш сервер;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(MAIN_PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    printf("Отправляем запрос серверу\n");

    while(1){

        printf("Напишем сереверу число (1 - минуты; 2 - секунды; 3 - часы): ");
        fgets(num_char, 5, stdin);
        num_char[strcspn(num_char, "\n")] = '\0';
        num = atoi(num_char);


        amount_byte = sendto(fd_serv, (void *)&num, sizeof(num), 0, (struct sockaddr *)&serv, len);
            error_func(amount_byte);

        amount_byte = recvfrom(fd_serv, (void *)&num, sizeof(num), 0, NULL, NULL);
            error_func(amount_byte);
        
        if(0 == amount_byte){
            break;
        } 

        if(-1 == num){
            printf("Такого запроса не существует\n");
        } else {
            printf("Сейчас: %d\n", num);
        }

    }

    exit(EXIT_SUCCESS);
}