#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define MAIN_PORT 3452

int main(void){

    struct sockaddr_in serv;
    //- дескриптор сокета;
    int fd_serv;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- буфер для записи;
    int num;
    char num_char[10];

    fd_serv = socket(AF_INET, SOCK_STREAM, 0);
        error_func(fd_serv);

    //-описывам наш сервер;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(MAIN_PORT);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    status = connect(fd_serv, (struct sockaddr *)&serv, sizeof(serv));
        error_func(status);
    
    amount_byte = recv(fd_serv, (void *)&serv, sizeof(serv), 0);
            error_func(amount_byte);
    
    while(1){

        printf("Напишем сереверу число: ");
        fgets(num_char, 5, stdin);
        num_char[strcspn(num_char, "\n")] = '\0';
        num = atoi(num_char);


        amount_byte = send(fd_serv, (void *)&num, sizeof(num), 0);
            error_func(amount_byte);

        amount_byte = recv(fd_serv, (void *)&num, sizeof(num), 0);
            error_func(amount_byte);
        
        if(0 == amount_byte){
            break;
        }
        
        printf("Сервер разделил и получилось число: %d\n", num);
    }

    exit(EXIT_SUCCESS);
}