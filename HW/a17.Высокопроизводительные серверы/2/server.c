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
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>


#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define AMOUNT_MATH_SERV 10 
#define NAME_FILE_KEY "key_sem"
#define MAIN_PORT 3452

//- функция для потока, которая будет делить числа на два;
void *math_server(void *arg);

//- условные переменные для потоков, которые ожидают, что им дадут
//поработать с клиентов;
pthread_cond_t condition[AMOUNT_MATH_SERV] = {
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER,
                                                PTHREAD_COND_INITIALIZER
};  

//- дескрипторы для клиентов;
int fd_clients[AMOUNT_MATH_SERV];
//- количетсво потоков поддерживаемые сервером;
pthread_t index_pthread[AMOUNT_MATH_SERV];
//- дескрипторы для потоков, обрабатывающие клиентов;
int fd_math_serv[AMOUNT_MATH_SERV];
//- идентификатор еще не занятого сервера;
int index_free_serv = 0;
//- начальный порт;
int port_serv[AMOUNT_MATH_SERV][2] = {{5689,0},{5690,0},{5691,},{5692,0},{5693,0},{5694,0},{5695,0},{5696,0},{5697,0},{5698,0}};
//- принимаем дескриптор;
int invite_fd;
//- ключ для семафоров;
key_t key_sem;
//- идентификатор семафора;
int index_sem;

//- мьютекст по обработки новых/выходящих клиентов
pthread_mutex_t mutex_clients =  PTHREAD_MUTEX_INITIALIZER;

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
    int i = 0;

    

    //- сначала создадим семафор, чтобы заблокирвоать процессы в
    //ожидании клиентов
    key_sem = ftok(NAME_FILE_KEY, 45);
    index_sem = semget(key_sem, 15, IPC_CREAT | 0666);
        error_func(index_sem);

    //- создадим пул обслуживающих серверов;
    for(i = 0; i < AMOUNT_MATH_SERV; i++){

        fd_math_serv[i] = socket(AF_INET, SOCK_STREAM, 0);
            error_func(fd_sock);
        
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port_serv[i][0]);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");

        pthread_create(&index_pthread[i], NULL, math_server, (void *)&i);

    }

    //-описывам наш сервер;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(MAIN_PORT);
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

        pthread_mutex_lock(&mutex_clients);

        //- ищем свободный обрабатывающий сервер;
        for(i = 0; i < AMOUNT_MATH_SERV; i++){
            if(0 == port_serv[i][1]){
                break;
            }
        }

        //- прикрепляем нового клиента к серверу;
        fd_clients[i] = invite_fd;

        //- обновляем нужные дынные перед отправкой пользователю;
        serv.sin_family = AF_INET;
        serv.sin_port = htons(port_serv[i][0]);
        serv.sin_addr.s_addr = inet_addr("127.0.0.1");
        //-отправляем пользователю информацию об нужном потоке;
        amount_byte = send(fd_clients[index_free_serv], (void *)&serv, sizeof(serv), 0);
            error_func(amount_byte);

        //- говорим обрабатывающему серверу, чтобы тот занялся клиентом;
        printf("текущие значение семафора %d установлено в %d\n",i,semctl(index_sem, i, GETVAL));

        status = semctl(index_sem, i, SETVAL, 0);
            error_func(status);

        pthread_mutex_unlock(&mutex_clients);

        printf("***\nЕще один клиент присоединился к серверу\n***\n");
    }

    for(i = 0; i < AMOUNT_MATH_SERV; i++){
        pthread_join(index_pthread[i], NULL);
    }

    close(fd_sock);

    exit(EXIT_SUCCESS);
}

void *math_server(void *arg){

    //- идентификатор семафора
    key_t key_sem;
    int index_sem;
    //- индекс обрабатывающего потока в системе;
    int index_thread = *((int *)arg);
    //- то число, которое нужно будет разделить на два;
    int num;
    int i = 0;
    ssize_t amount_byte;
    int fd_client_math_serv;
    //- для ловки ошибок;
    int status;

    //- структуры, чтобы начать и закончить работать с клиентом
    struct sembuf for_unlock[2] = {{index_thread,1,0},{index_thread,0,0}};

    //- тут тоже создадим семафор;
    key_sem = ftok(NAME_FILE_KEY, 45);
    index_sem = semget(key_sem, 15, 0666);

    while(1){

        sleep(1);

        //- ждем по индексу потока, пока нам предоставят пользователя;
        printf("Поток %d ждет клиента\n", fd_math_serv[index_thread]);
        //- блокируемся, пока нет клиента
        status = semop(index_sem, for_unlock, 2);
            error_func(status);
        printf("К потоку %d присоединился клиент\n", fd_math_serv[index_thread]);

        
        fd_client_math_serv = fd_clients[index_thread];

        while(1){
            
            sleep(1);

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

        close(fd_clients[index_thread]);
        fd_clients[index_thread] = 0;

        port_serv[index_thread][1] = 0;

        pthread_mutex_unlock(&mutex_clients);
    }
}