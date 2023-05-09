#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#define PATH_SOCKET "/path_serv_forever"
#define PATH_CLIENT "/path_client_forever"

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    struct sockaddr_un serv, client;
    //- дескриптор сокета;
    int fd_serv, fd_client;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    char sms[100];

    fd_serv = socket(AF_LOCAL, SOCK_DGRAM, 0);

    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, PATH_SOCKET);

    client.sun_family = AF_LOCAL;
    strcpy(client.sun_path, PATH_CLIENT);

    status = bind(fd_serv, (struct sockaddr *)&client, sizeof(client));
        error_func(status);

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

    unlink(PATH_CLIENT);

    exit(EXIT_SUCCESS);
}