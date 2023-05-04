#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>

#define PATH_SOCKET "/path_3"

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    struct sockaddr_un serv;
    struct sockaddr_un client;
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

    //- создаем сокет;
    fd_sock = socket(AF_LOCAL, SOCK_DGRAM, 0);
        error_func(fd_sock);

    //-описывам наш сервер;
    serv.sun_family = AF_LOCAL;
    strcpy(serv.sun_path, PATH_SOCKET);

    status = bind(fd_sock, (struct sockaddr *)&serv, sizeof(serv));
        error_func(status);
    
    printf("Начинаем прослушивать клиентов\n");

    status = listen(fd_sock, 5);
        error_func(status);
    
    len = sizeof(client);
    fd_clients = accept(fd_sock, (struct sockaddr *)&client, &len);
        error_func(fd_clients);

    printf("Клиент присоединился к серверу\n");

    while(1){

        amount_byte = recv(fd_clients, (void *)&buf, sizeof(buf), 0);
            error_func(amount_byte);

        if(!strcmp(buf,"exit")){
            break;
        }
        
        printf("Клиент пишет: %s", buf);
        printf("Отвечаем ему: ладно\n");

        strcpy(buf, "ok");

        amount_byte = send(fd_clients, (void *)buf, sizeof(buf), 0);
            error_func(amount_byte);
    }

    close(fd_clients);
    close(fd_sock);

    unlink(PATH_SOCKET);

    exit(EXIT_SUCCESS);
}