#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define AMOUNT_MATH_SERV 10 

//- функция для потока, которая будет делить числа на два;
void *math_server(void *arg);

//- дескрипторы для клиентов;
int fd_clients[AMOUNT_MATH_SERV];
//- количетсво потоков поддерживаемые сервером;
pthread_t index_pthread[AMOUNT_MATH_SERV];
//- дескрипторы для потоков, обрабатывающие клиентов;
int fd_math_serv[AMOUNT_MATH_SERV];
//- идентификатор еще не занятого сервера;
int index_free_serv = 0;
//- начальный порт
int port_serv[AMOUNT_MATH_SERV] = {5689,5690,5691,5692,5693,5694,5695,5696,5697,5698};
//- принимаем дескриптор
int invite_fd;

//- мьютекст по обработки новых/выходящих клиентов
pthread_mutex_t mutex_clients = PTHREAD_MUTEX_INITIALIZER;

int main(void){

    struct sockaddr_in serv;
    struct sockaddr_in client;
    //- дескриптор сокета;
    int fd_sock;
    //- для проверки правильности завершения программы;
    int status;
    ssize_t amount_byte;
    //- тут храним размер для фукнции accept();
    socklen_t len;
    //- создаем сокет;
    fd_sock = socket(AF_INET, SOCK_STREAM, 0);
        error_func(fd_sock);
    //- индекс для циклов;
    int i;

    //-описывам наш сервер;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(3456);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    status = bind(fd_sock, (struct sockaddr *)&serv, sizeof(serv));
        error_func(status);
    
    printf("Начинаем прослушивать клиентов\n");

    //- создаем очередь для подключения клиентов;
    status = listen(fd_sock, 5);
        error_func(status);
    
    len = sizeof(client);

    while(1){

        sleep(1);

        invite_fd = accept(fd_sock, (struct sockaddr *)&client, &len);
            error_func(invite_fd);

        printf("Еще один клиент присоединился к серверу\n");

        pthread_mutex_lock(&mutex_clients);

        fd_clients[index_free_serv] = invite_fd;

        fd_math_serv[index_free_serv] = socket(AF_INET, SOCK_STREAM, 0);
            error_func(fd_sock);
        
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port_serv[index_free_serv]);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        pthread_create(&index_pthread[index_free_serv], NULL, math_server, (void *)&fd_clients[index_free_serv]);

        amount_byte = send(fd_clients[index_free_serv], (void *)&serv, sizeof(serv), 0);
            error_func(amount_byte);
        
        index_free_serv++;

        pthread_mutex_unlock(&mutex_clients);
    }

    for(i = 0; i < AMOUNT_MATH_SERV; i++){
        pthread_join(index_pthread[index_free_serv], NULL);
    }

    close(fd_sock);

    exit(EXIT_SUCCESS);
}



void *math_server(void *arg){

    //- дескриптор клиента;
    int fd_client_math_serv = *((int *)arg);
    //- то число, которое нужно будет разделить на два;
    int num;
    int i = 0;
    ssize_t amount_byte;

    while(1){
        amount_byte = recv(fd_client_math_serv, (void *)&num, sizeof(num), 0);
            error_func(amount_byte);

        if(-1 == num){
            break;
        }
            
        printf("Клиент прислал цифру: %d\n", num);

        num = num / 2;

        amount_byte = send(fd_client_math_serv, (void *)&num, sizeof(num), 0);
        error_func(amount_byte);
    }

    pthread_mutex_lock(&mutex_clients);

    for(i = 0; i < AMOUNT_MATH_SERV; i++){
        if(fd_client_math_serv == fd_clients[i]){
            break;
        }
    }

    num =  port_serv[i];
    port_serv[i] = port_serv[index_free_serv - 1];
    port_serv[index_free_serv - 1] = num;

    close(fd_clients[i]);
    fd_clients[i] = fd_clients[index_free_serv - 1];
    fd_clients[index_free_serv - 1] = 0;

    index_pthread[i] = index_pthread[index_free_serv - 1];
    index_pthread[index_free_serv - 1] = 0;

    fd_math_serv[i] = fd_math_serv[index_free_serv - 1];
    fd_math_serv[index_free_serv - 1] = 0;
    
    index_free_serv--;

    pthread_mutex_unlock(&mutex_clients);
}