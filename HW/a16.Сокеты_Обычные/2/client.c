#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PATH_SOCKET "/path_3"

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    struct sockaddr_un serv;
    //- дескриптор сокета;
    int fd_serv;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    char sms[100];

    fd_serv = socket(AF_LOCAL, SOCK_STREAM, 0);

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, PATH_SOCKET);

    status = connect(fd_serv, (struct sockaddr *)&serv, sizeof(serv));
        error_func(status);
    
    while(1){

        printf("Напишем сереверу: ");
        fgets(sms, 100, stdin);
        sms[strcspn(sms, "\n")] = '\0';

        amount_byte = send(fd_serv, (void *)&sms, sizeof(sms), 0);
            error_func(amount_byte);

        amount_byte = recv(fd_serv, (void *)&sms, sizeof(sms), 0);
            error_func(amount_byte);
        
        if(0 == amount_byte){
            break;
        }
        
        printf("Сервер отвечает: %s\n", sms);
    }

    exit(EXIT_SUCCESS);
}