#include <stdio.h>
#include <sys/socket.h> /*семейство протоколов*/
#include <sys/un.h> /*подключение функции сокетов*/
#include <stdlib.h>
#include <unistd.h>
#include <string.h>


int main(){

    struct sockaddr server_addr;
    memset(&server_addr, 0, sizeof(struct sockaddr));

}
