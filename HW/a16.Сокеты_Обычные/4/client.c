#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    struct sockaddr_in serv, client;
    //- дескриптор сокета;
    int fd_serv, fd_client;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    char sms[100];

    fd_serv = socket(AF_INET, SOCK_DGRAM, 0);

    serv.sin_family = AF_INET;
    serv.sin_port = htons(3457);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    //- используем connect() чтобы не использовать каждый раз sendto()
    status = connect(fd_serv, (struct sockaddr *)&serv, sizeof(serv));
        error_func(status);
    
    while(1){

        printf("Напишем сереверу: ");
        fgets(sms, 100, stdin);
        sms[strcspn(sms, "\n")] = '\0';

        amount_byte = send(fd_serv, (void *)&sms, sizeof(sms), 0);
            error_func(amount_byte);

        if(!strcmp(sms,"exit")){
            break;
        }

        amount_byte = recv(fd_serv, (void *)&sms, sizeof(sms), 0);
            error_func(amount_byte);
        
        if(0 == amount_byte){
            break;
        }
        
        printf("Сервер отвечает: %s\n", sms);
    }

    close(fd_serv);

    exit(EXIT_SUCCESS);
}