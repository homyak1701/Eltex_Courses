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
    //- для включения multicast;
    struct ip_mreqn struct_multicust;
    struct_multicust.imr_address.s_addr = INADDR_ANY;
    struct_multicust.imr_multiaddr.s_addr = inet_addr("224.0.0.3");
    //- так как мы будем работать локально, то мы будем использовать ноль; 
    struct_multicust.imr_ifindex = 0;

    fd_client = socket(AF_INET, SOCK_DGRAM, 0);

    //- позволяем сокету делать широковещательную рассылку;
    status = setsockopt(fd_client, IPPROTO_IP, IP_ADD_MEMBERSHIP, &struct_multicust, sizeof(struct_multicust));
        error_func(status);

    client.sin_family = AF_INET;
    client.sin_port = htons(7777);
    client.sin_addr.s_addr = inet_addr("224.0.0.3");

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