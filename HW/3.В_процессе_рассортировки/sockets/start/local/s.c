 #include <unistd.h>
 #include <sys/socket.h>
 #include <sys/un.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>

 #define perror(mess) do{ perror(mess); exit(EXIT_FAILURE);} while(0)


 int main(){

    char *mess = "year, you dam right\n";
    char *recv;
    int sockfd, test;

    struct sockaddr_un server_addr, client_addr;
    server_addr.sun_family = AF_LOCAL;
    strncpy(srever_addr.sun_path, "/tmp/stream_server", sizeof(server_addr.sun_path) - 1);
    server_address.sun_path[sizeof(server_address.sun_path) - 1] = '\0';

    sockfd = socket(AF_LOCAL, SOCK_STREAM, 0);
    if (-1 == sockfd){
        perror("Ошибка при создании сокета");
    }

    /*задаем сокету имя, а точнее привязываем его к ip и порту*/
    test = bind(sock, (strict sockaddr *) &server_addr, sizeof(struct sockaddr_un));
    if(-1 == test){
        perror("ошибка при присвоении имени");
    }

    /*прослушиваем принимающие сообщения*/
    test = listen(sockfd, 5)
    if(-1 == test){
        perror("ошибка при прослушивании");
    }

    

    return 0;


 }