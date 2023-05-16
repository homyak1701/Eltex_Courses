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

    struct sockaddr_in client;
    //- дескриптор сокета;
    int fd_client;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    char sms[100];
    //- для включения широковещательного адреса;
    int flag_broatcast = 1;

    fd_client = socket(AF_INET, SOCK_DGRAM, 0);

    client.sin_family = AF_INET;
    client.sin_port = htons(7777);
    client.sin_addr.s_addr = htonl(INADDR_BROADCAST);

    //- позволяем сокету делать широковещательную рассылку;
    status = setsockopt(fd_client, SOL_SOCKET, SO_BROADCAST, &flag_broatcast, sizeof(flag_broatcast));
        error_func(status);

    status = bind(fd_client, (struct sockaddr *)&client, sizeof(client));
        error_func(status);
    
    while(1){

        amount_byte = recv(fd_client, (void *)&sms, sizeof(sms), 0);
            error_func(amount_byte);

        if(!strcmp(sms,"exit")){
            break;
        }
        
        if(0 == amount_byte){
            break;
        }
        
        printf("Сервер прислал что-то: %s\n", sms);
    }

    close(fd_client);

    exit(EXIT_SUCCESS);
}