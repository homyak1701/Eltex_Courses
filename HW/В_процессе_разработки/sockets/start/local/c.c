#include <stdio.h>
#include <sys/socket.h> /*семейство протоколов*/
#include <sys/un.h> /*подключение функции сокетов*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define perror(msg) do{perror(msg); exit(EXIT_FAILURE);} while(0)

int main(){
    char *mess = "there is a penetration";
    char *recv_mess;
    int sockfd; /*файловый дескриптор для сокетов*/
    int connection_status; /*переменная для проверки статуса подключения*/
    struct sockaddr_un server_addr; /*структура для содержания сокетного адреса любого типа*/
    //memset(&server_addr, 0, sizeof(struct sockaddr));

    /*определяем семейство сокетов данный сокет испл. для локальной передачи*/
    /*AF_LOCAL / AF_UNIX одно и то же*/
    server_addr.sun_family = AF_LOCAL;   
    /*указываем путь*/
    /*так как в массив разово нельзя положить строку мы воспользуемся функцией strnspy*/
    strncpy(server_addr.sun_path, "/tmp/stream_server", sizeof(server_addr.sun_path) - 1);
    /*зануляем последний байт*/
    server_addr.sun_path[sizeof(server_addr.sun_path) - 1] = '\0';

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0); /*создание сокета на сервер*/
    if(-1 == sockfd){
        perror("Создание сокета провалилось");
    }
    /*производим подключение к серверу*/
    connection_status = connect(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if(-1 == connection_status){
        perror("Ошибка подключения");
    }

    send(sockfd, mess, sizeof(mess), 0); /*откравляем сообщение на сервер*/
    recv(sockfd, &recv_mess, sizeof(recv_mess), 0); /*ожидание получения обратного сообщения*/

    printf("--> %s\n", recv_mess);

    close(sockfd);
    
    return 0;
}

