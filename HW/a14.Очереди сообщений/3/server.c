#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ioctl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <dirent.h>
#include <errno.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define PRIOR_SEND_QUERTY_OR_SMS 5 //- приоритет, по которому отправляются запросы на сервер;
#define PRIOR_RECEPTION 6 //- приоритет, по которому сервер отвечает пользователю, что он вошел в сессию;
#define MAX_USERS 10//- максимальное количетсво пользователей в сессии;
#define MAX_SMS 40 //- максимальное количетсво смс на экране;

//- мьютекс будем использовать, когда будем добавлять нового пользователя;
pthread_mutex_t mutex_new_user;

//- структура, которая будет содержать сообщение пользователя и рассылку всем пользователям;
struct send_and_request{
    long mtype;
    char sms[50];
    char users[10][20];
    int amount_users; //количетсво пользователей в сети;
    int num_user; //- номер пользователя на сервере;
    int type_sms; //- тип сообщения
                    //-1 - ответ: сервер переполнен;
                    // 0 - запрос на вход в чат / новое смс;
                    // 1 - запрос на выход / отключение от сервера;
                    // 2 - ошибка соединения;
                    // 3 - отправить новое сообщение;
                    // 4 - ответ: новый пользователь (пользоватеьл вышел);
                    // 5 - ответ: новое сообщение;
                    // 6 - отключить вервер;
    int priority; //- сюда будет возвращаться приоритет, по которому пользователь будет получать смс
                    //- если чат переполнен то возвращается -1;
};

int main(void){

    key_t key_sms;
    //- для идентификатора очереди сообщения;
    int id_sms;

    //- одна структура используется, чтобы принимать сообщения;
    //- другая хранит всю информацию о сеансе;
    struct send_and_request send_and_request_users_data;
    struct send_and_request send_and_request;
    //- название файла;
    char *name_file = "file_key.txt"; 
    //- для проверки завершения дочернего процесса;
    int status;
    ssize_t status_rcv;
    //- идентификатор последнего пользователя;
    int last_user = 0;
    //- чтобы понять какие приориеты заняты;
    int mas_prio[10] = {0,0,0,0,0,0,0,0,0,0};
    int free_prio = 10;
    //- счетчик и помошник;
    int i = 0, i2 = 0;
    //- идентификатор работы сервера
    int work = 1;
    //- индентификатор пользователя на сервере; 
    int num_user_server;

    key_sms = ftok(name_file, 6);    
        error_func(key_sms);

    id_sms = msgget(key_sms, IPC_CREAT | 0666);
        error_func(id_sms); 

    for(i = 5; i < 20; i++ ){
        while(msgrcv(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), i, IPC_NOWAIT) > 0);
    }
    

    send_and_request.amount_users = 0;
    send_and_request_users_data.amount_users = 0;

    printf("Сервер запустился, начинает работу\n");

    //- начинаем принимать заявки от пользователей;
    while(1 == work){
        sleep(1);
        status_rcv = msgrcv(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), PRIOR_SEND_QUERTY_OR_SMS, 0);
            error_func(status_rcv);

        switch(send_and_request.type_sms){
            //- запрос на вход;
            case 0:
                printf("Новый пользователь %s пытает зайти\n", send_and_request.users[0]);
                for(i = 0; i < MAX_USERS; i++){
                    if((0 == mas_prio[i]) && (20 > free_prio)){

                        //- увеличиваем счетчик максимального количетсво пользователей в сесии;

                        last_user++;
                        free_prio++;

                        i2 = i;

                        //- отмечаем сколько всего пользователей в системе;
                        send_and_request_users_data.amount_users = last_user;
                        //- добавляем нового пользователя в общую структуру данных;
                        strcpy(send_and_request_users_data.users[i],send_and_request.users[0]);
                        //отправляем по приоритету нового пользователя в системе;
                        send_and_request_users_data.type_sms = 4;
                        //- отправляем всем, что появился новый пользователь;
                        for(i = 0; i < MAX_USERS; i++){
                            if(0 != mas_prio[i]){
                                send_and_request_users_data.mtype = mas_prio[i];
                                send_and_request_users_data.priority = mas_prio[i];
                                send_and_request_users_data.num_user = i;
                                status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                                    error_func(status);
                            }
                        }

                        //- отправляем приоритет новому пользователю, по которому, пользователь сможет получать сообщения;
                        send_and_request_users_data.priority = free_prio;
                        //- отмечаем, что приоритет был занят;
                        mas_prio[i2] = send_and_request_users_data.priority;
                        //- отмечаем идентификатор пользователя;
                        send_and_request_users_data.num_user = i2;

                        send_and_request_users_data.mtype = PRIOR_RECEPTION;
                        status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                            error_func(status);
                        //- выходим из цикла, когда добавили пользователя;
                        break;

                    }else if(i == 9){
                        //- отправляем пользователю, что сессия переполнена;
                        send_and_request.priority = -1;
                        send_and_request.mtype = PRIOR_RECEPTION;

                        status = msgsnd(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), 0);
                            error_func(status);
                        break;
                    }
                }
                break;
            //- запрос на выход;
            case 1:

                for(i = 0; i < MAX_USERS; i++){
                    if(mas_prio[i] == send_and_request.priority){
                        num_user_server = i;
                    }
                }
                

                printf("Пользователь %s пытается выйти из сессии\n", send_and_request_users_data.users[num_user_server]);
                //- отправляем процессу пользователя, чтобы он вышел;
                send_and_request_users_data.mtype = mas_prio[num_user_server];
                send_and_request_users_data.type_sms = -1;
                status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                        error_func(status);

                //последнего пользоваетля переместим на место вышедшего пользователя;
                strcpy(send_and_request_users_data.users[num_user_server], send_and_request_users_data.users[last_user - 1]);
                //занулим пространство последнего пользователя;
                memset(send_and_request_users_data.users[last_user - 1], 0, sizeof(send_and_request_users_data.users[last_user - 1]));
                //- удаляем пользователя из массива приоритетов;
                mas_prio[num_user_server] = mas_prio[last_user - 1];
                mas_prio[last_user - 1] = 0;
                //- увеличиваем доступное количетсво пользователей в сесии;
                last_user--;
                send_and_request_users_data.amount_users--;
                //отправляем по приоритету список пользователей в сети;
                send_and_request_users_data.type_sms = 4;

                for(i = 0; i < MAX_USERS; i++){
                    if(0 != mas_prio[i]){
                        send_and_request_users_data.mtype = mas_prio[i];
                        send_and_request_users_data.priority = mas_prio[i];
                        send_and_request_users_data.num_user = i;
                        status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                        error_func(status);
                    }
                }

                break;
            case 3:
                for(i = 0; i < MAX_USERS; i++){
                    if(mas_prio[i] == send_and_request.priority){
                        num_user_server = i;
                    }
                }

                for(i = 0; i < MAX_USERS; i++){
                    if(mas_prio[i] == send_and_request.priority){
                        num_user_server = i;
                    }
                }
                
                memset(send_and_request_users_data.sms, 0, sizeof(send_and_request_users_data.sms));
                strcat(send_and_request_users_data.sms, send_and_request_users_data.users[num_user_server]);
                strcat(send_and_request_users_data.sms, ": ");
                strcat(send_and_request_users_data.sms, send_and_request.sms);

                printf("Пользователь %s отправляет сообщение\n", send_and_request_users_data.users[num_user_server]);

                send_and_request_users_data.type_sms = 5;

                for(i = 0; i < MAX_USERS; i++){
                    if(0 != mas_prio[i]){
                        send_and_request_users_data.mtype = mas_prio[i];
                        send_and_request_users_data.priority = mas_prio[i];
                        send_and_request_users_data.num_user = i;
                        status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                            error_func(status);
                    }
                }


                break;
            //- выключение сервера
            case 6:
                printf("Пользователь %s пытается выключить сервер\n", send_and_request.users[0]);
                send_and_request_users_data.amount_users = -1;

                for(i = 0; i < MAX_USERS; i++){
                    if(0 != mas_prio[i]){
                        send_and_request_users_data.mtype = mas_prio[i];
                        send_and_request_users_data.priority = mas_prio[i];
                        send_and_request_users_data.num_user = i;
                        status = msgsnd(id_sms, &send_and_request_users_data, sizeof(send_and_request_users_data) - sizeof(send_and_request_users_data.mtype), 0);
                            error_func(status);
                    }
                }
                
                work = 0;

                break;
            default:
                breal;
        }

    } 
    exit(EXIT_SUCCESS);
}
