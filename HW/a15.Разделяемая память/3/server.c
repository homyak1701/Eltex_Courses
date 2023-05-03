#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <signal.h>
#include <curses.h>
#include <dirent.h>
#include <time.h>
#include <malloc.h>
#include <sys/ioctl.h>

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

#define PRIOR_SEND_QUERTY_OR_SMS 5 //- приоритет, по которому отправляются запросы на сервер;
#define PRIOR_RECEPTION 6 //- приоритет, по которому сервер отвечает пользователю, что он вошел в сессию;
#define MAX_USERS 10//- максимальное количетсво пользователей в сессии;
#define MAX_SMS 40 //- максимальное количетсво смс на экране;

#define KEY_NAME_TO_SERVER "key_to_server"
#define KEY_NAME_FROM_SERVER "key_from_server"

//- структура, которая будет содержать сообщение пользователя и рассылку всем пользователям;
struct send_and_request{
    char sms[50];
    char users[10][20];
    int amount_users; //количетсво пользователей в сети;
    int num_user[10]; //- порядкое номера пользователей на сервере;
    int type_sms; //- тип сообщения
                    //-1 - ответ: сервер переполнен;
                    // 0 - запрос на вход в чат / новое смс;
                    // 1 - запрос на выход / отключение от сервера;
                    // 2 - ошибка соединения;
                    // 3 - отправить новое сообщение;
                    // 4 - ответ: новый пользователь (пользоватеьл вышел);
                    // 5 - ответ: новое сообщение;
                    // 6 - отключить вервер;
    int answer; //- для ответов сервера;
};

int main(void){

    key_t key_sms_from_server, key_sms_to_server;
    //- для идентификатора разделяемой памяти и семафорров;
    int id_shm_get,id_shm_send, id_sem;

    //- одна структура используется, чтобы принимать сообщения;
    //- другая хранит всю информацию о сеансе;
    struct send_and_request *point_shm_send;
    struct send_and_request *point_shm_get;
    //- для проверки завершения дочернего процесса;
    int status;
    ssize_t status_rcv;
    //- идентификатор последнего пользователя;
    int last_user = 0;
    //- счетчик и помошник;
    int i = 0, i2 = 0;
    //- идентификатор работы сервера
    int work = 1;
    //- индентификатор пользователя на сервере; 
    int num_user_server;
    //- сервер блокируется, пока не придут новое сообщение;
    struct sembuf lock_sms = {1,0,0};
    struct sembuf lock_get_sms[3] = {{3,0,0},{2,1,0},{4,-1,0}};

    //- создаем ключи для разделяемой памяти и для семафоров;
    key_sms_from_server = ftok(KEY_NAME_FROM_SERVER, 6);    
        error_func(key_sms_from_server);
    key_sms_to_server = ftok(KEY_NAME_TO_SERVER, 6);    
        error_func(key_sms_to_server);

    //1) семафор для разрешения отправки сообщения на сервер;
    //2) семафор для ожидания сообщения;
    //3) уведомляем пользователей, что пришло новое сообщение;
    id_sem = semget(key_sms_from_server, 10, IPC_CREAT | 0666);
        error_func(id_sem); 

    //- получаем идентификатор на область памяти;
    id_shm_get = shmget(key_sms_to_server, sizeof(struct send_and_request), IPC_CREAT | 0666);
        error_func(id_shm_get); 
    id_shm_send = shmget(key_sms_from_server, sizeof(struct send_and_request), IPC_CREAT | 0666);
        error_func(id_shm_send); 
    
    //- получаем указатель на разделяемую память и защищаем эту область;
    point_shm_send = (struct send_and_request *)shmat(id_shm_send, NULL, 0);
    point_shm_get = (struct send_and_request *)shmat(id_shm_get, NULL, 0);

    memset(point_shm_get, 0, sizeof(struct send_and_request));
    memset(point_shm_send, 0, sizeof(struct send_and_request));

    for(i = 0; i < 10; i++){
        status = semctl(id_sem, i, SETVAL, 15);
            error_func(status);
    }

    printf("Сервер запустился, начинает работу\n");

    //- начинаем принимать заявки от пользователей;
    while(1 == work){
        sleep(1);
        status = semctl(id_sem, 1, SETVAL, 1);
            error_func(status);
        //- разрешаем одному из пользователей прислать новое сообщение;
        status = semctl(id_sem, 0, SETVAL, 0);
            error_func(status);
        //- ждем, пока кто-нибудь принесет сообщение;
        status = semop(id_sem, &lock_sms, 1);
            error_func(status);

        switch(point_shm_get->type_sms){
            //- запрос на вход;
            case 0:
                printf("Новый пользователь %s пытает зайти\n", point_shm_get->users[0]);
                if(10 >= point_shm_send->amount_users){

                    //- увеличиваем счетчик максимального количетсво пользователей в сесии;
                    point_shm_send->amount_users++;
                    //- добавляем нового пользователя в общую структуру данных;
                    strcpy(point_shm_send->users[last_user],point_shm_get->users[0]);
                    last_user++;
                    //отправляем по приоритету нового пользователя в системе;
                    point_shm_send->type_sms = 4;
                    //- отправляем всем, что появился новый пользователь;
                    //-ставим значения, чтобы ждать, пока пользователи прочитают сообщения;

                }else {
                    //- что будет, если сервер переполнен
                    point_shm_send->type_sms = -4;
                }

                status = semctl(id_sem, 3, SETVAL, last_user);
                    error_func(status);
                status = semctl(id_sem, 4, SETVAL, 1); 
                    error_func(status);
                //- разрешаем пользователям читать сообщения;
                status = semctl(id_sem, 2, SETVAL, 0); 
                    error_func(status);
                //- Ждем, пока все пользователи прочитают сообщения;
                status = semop(id_sem, lock_get_sms, 3);
                    error_func(status);

                break;

            //- запрос на выход;
            case 1:
                printf("Пользователь %s пытается выйти из сессии\n", point_shm_get->users[0]);
            
                //- ищем пользователя на сервере;
                for(i = 0; i < 10; i++){
                    if(!strcmp(point_shm_get->users[0], point_shm_send->users[i])){
                        i2 = i;
                    }
                }

                //последнего пользоваетля переместим на место вышедшего пользователя;
                strcpy(point_shm_send->users[i2], point_shm_send->users[last_user - 1]);
                //занулим пространство последнего пользователя;
                memset(point_shm_send->users[last_user - 1], 0, sizeof(point_shm_send->users[last_user - 1]));
                point_shm_send->amount_users--;
                //отправляем по приоритету список пользователей в сети;
                point_shm_send->type_sms = -4;

                status = semctl(id_sem, 3, SETVAL, last_user);
                    error_func(status);
                status = semctl(id_sem, 4, SETVAL, 1); 
                    error_func(status);
                //- разрешаем пользователям читать сообщения;
                status = semctl(id_sem, 2, SETVAL, 0); 
                    error_func(status);
                //- Ждем, пока все пользователи прочитают сообщения;
                status = semop(id_sem, lock_get_sms, 3);
                    error_func(status);

                break;
            case 3:

                memset(point_shm_send->sms, 0, sizeof(point_shm_send->sms));
                strcat(point_shm_send->sms, point_shm_get->users[0]);
                strcat(point_shm_send->sms, ": ");
                strcat(point_shm_send->sms, point_shm_get->sms);

                point_shm_send->type_sms = 5;

                status = semctl(id_sem, 3, SETVAL, last_user);
                    error_func(status);
                status = semctl(id_sem, 4, SETVAL, 1); 
                    error_func(status);
                //- разрешаем пользователям читать сообщения;
                status = semctl(id_sem, 2, SETVAL, 0); 
                    error_func(status);
                //- Ждем, пока все пользователи прочитают сообщения;
                status = semop(id_sem, lock_get_sms, 3);
                    error_func(status);

                break;
            //- выключение сервера
            case 6:
                printf("Пользователь %s пытается выключить сервер\n", point_shm_get->users[0]);
                point_shm_send->type_sms = -1;

                status = semctl(id_sem, 3, SETVAL, last_user);
                    error_func(status);
                status = semctl(id_sem, 4, SETVAL, 1); 
                    error_func(status);
                //- разрешаем пользователям читать сообщения;
                status = semctl(id_sem, 2, SETVAL, 0); 
                    error_func(status);
                //- Ждем, пока все пользователи прочитают сообщения;
                status = semop(id_sem, lock_get_sms, 3);
                    error_func(status);
                
                work = 0;

                break;

            default:
            
                break;
        }

    } 
    exit(EXIT_SUCCESS);
}
