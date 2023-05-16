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
    int fd_sock;
    //- для проверки правильности завершения программы;
    int status;
    ssize_t amount_byte;
    //- дескрипторы для клиентов;
    int fd_clients;
    char buf[100];
    //- тут храним размер;
    socklen_t len;
    len = sizeof(client);
   
    //- создаем сокет;
    fd_sock = socket(AF_INET, SOCK_DGRAM, 0);
        error_func(fd_sock);
    
    //-описывам наш сервер;
    client.sin_family = AF_INET;
    client.sin_port = htons(7777);
    client.sin_addr.s_addr = inet_addr("224.0.0.3");
    
    printf("Сделали сервер, теперь напишем всем пользователям в сети\n");

    while(1){

        printf("Напишем всем клиентам: ");
        fgets(buf, 100, stdin);
        buf[strcspn(buf, "\n")] = '\0';

        amount_byte = sendto(fd_sock, (void *)&buf, strlen(buf), 0, (struct sockaddr *)&client, len);
            error_func(amount_byte);
        
        if(!strcmp(buf,"exit")){
            break;
        }

    }

    close(fd_sock);

    exit(EXIT_SUCCESS);
}