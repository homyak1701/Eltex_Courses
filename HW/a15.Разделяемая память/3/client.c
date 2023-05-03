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

//- функция, что делает дефолтные надписи в окнах;
void start_logo(WINDOW *wnd_see_sms, WINDOW *wnd_urers, WINDOW *wnd_send_sms);

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)


#define PRIOR_SEND_QUERTY_OR_SMS 5 //- приоритет, по которому отправляются запросы на сервер;
#define PRIOR_RECEPTION 6 //- приоритет, по которому мы принимает ответ от сервера;
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
};


void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

int main(void){


    //- создадим указатели на окна;
    WINDOW *wnd_send_sms;
    WINDOW *wnd_urers;
    WINDOW *wnd_see_sms;

    //- значения координат;
    int x_names = 0, y_names = 0;
    int x_sms = 0, y_sms = 0;
    //- буфер для своего имени
    char buf_name[20];
    char buf_sms[28];
    //- счетчик для циклов
    int i;
    //- указатель на область разделенной памяти;
    struct send_and_request *point_shm_send;
    struct send_and_request *point_shm_get;
    key_t key_sms_from_server, key_sms_to_server;
    //- для идентификаторов разделяемой памяти и семафоров;
    int id_shm_get, id_shm_send, id_sem;

    //- для создания нового процесса;
    //- для разделения поля имен и сообщений;
    pid_t pid;
    //- для проверки завершения дочернего процесса;
    int status;
    //- для проверки есть ли пользователь на серевере или нет
    int yes_or_no;
    //- ожидание, что сервер даст возможность отправить сообщение;
    struct sembuf lock_1[2] = {{0,0,0},{0,1,0}};
    //- уведомляем сервер, что послали новое сообщение;
    struct sembuf lock_2 = {1,-1,0};
    //- уведомление от сервера, что пришло новое сообщение
    struct sembuf lock_3 = {2,0,0};
    //- уведомляем сервер, что прочитали сообщение
    struct sembuf lock_4 = {3,-1,0};
    struct sembuf lock_5 = {4,0,0};

    //- создаем ключи для разделяемой памяти и для семафоров;
    key_sms_from_server = ftok(KEY_NAME_FROM_SERVER, 6);    
        error_func(key_sms_from_server);
    
    key_sms_to_server = ftok(KEY_NAME_TO_SERVER, 6);    
        error_func(key_sms_from_server);

    //- получаем идентификатор на область памяти;
    id_shm_get = shmget(key_sms_from_server, sizeof(struct send_and_request), IPC_EXCL | 0666);
        error_func(id_shm_get);
    id_shm_send = shmget(key_sms_to_server, sizeof(struct send_and_request), IPC_EXCL | 0666);
        error_func(id_shm_send);

    //- получаем указатель на разделяемую память;
    point_shm_get = (struct send_and_request *)shmat(id_shm_get, NULL, 0);
    point_shm_send = (struct send_and_request *)shmat(id_shm_send, NULL, 0);

    //1) семафор для отправки сообщения на сервер;
    //2) семафор для принятия сообщения от сервера;
    //3) уведомление от сервера, что пришло новое сообщение;
    //4) увеомляем сервер, что прочитали сообщение;
    id_sem = semget(key_sms_from_server, 10, IPC_EXCL | 0666);
        error_func(id_sem); 

    //- инициализирем структуры данных ncurses и переводим терминал в нужный режим;
    initscr();
    //- устанавливаем обработчик сигнала SIGWINCH;
    signal(SIGWINCH, sig_winch);
    //- инициализируем управлением цветом;
    start_color();
    //- отключаем отображение курсора;
    curs_set(0);
    //- отключаем отображение символов на экране;
    noecho();

    //- создаем цветовые пары для каждого окна;
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    init_pair(2, COLOR_BLACK, COLOR_RED);
    init_pair(3, COLOR_RED, COLOR_GREEN);
    init_pair(4, COLOR_YELLOW, COLOR_GREEN);

    bkgd(COLOR_PAIR(1));
    refresh();

    //- создаем два окна, как в mc;
    wnd_send_sms = newwin((stdscr->_maxy/10) - 1,(stdscr->_maxx/4) - 3, (stdscr->_maxy/4) + 22, 3);
    wnd_urers = newwin((stdscr->_maxy/4) - 1,(stdscr->_maxx/6) - 1, 3, (stdscr->_maxx/3) - 10 + 20 + 30);
    wnd_see_sms = newwin((stdscr->_maxy/4) + 17, (stdscr->_maxx/4) - 3 + 20, 3, 3);

    wattron(wnd_send_sms, COLOR_PAIR(2));
    wbkgd(wnd_send_sms, COLOR_PAIR(2));

    wattron(wnd_urers, COLOR_PAIR(3));
    wbkgd(wnd_urers, COLOR_PAIR(3));

    wattron(wnd_see_sms, COLOR_PAIR(3));
    wbkgd(wnd_see_sms, COLOR_PAIR(3));

    wmove(wnd_see_sms, 4, 2);
    wprintw(wnd_see_sms, "Input your name");

    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);

    wrefresh(wnd_see_sms);
    wrefresh(wnd_urers);
    wrefresh(wnd_send_sms);
    //- включаем отображение сомволов в окне;
    echo();
    
    curs_set(1);

    //- в первый элемент массива имен запишем имя пользователя, которое отправим серверу;
    wmove(wnd_send_sms, 1, 1);

    wgetnstr(wnd_send_sms, buf_name, 20);
    //- после того, как пользователь что-то отправил сотрем все ненужно в поле;
    werase(wnd_send_sms);
    werase(wnd_urers);
    werase(wnd_see_sms);
    //- и обновим названия полей;
    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);  


    //- отправляем серверу запрос на вход в сессию
    status = semop(id_sem, lock_1, 2);
        error_func(status);
    point_shm_send->type_sms = 0;
    strcpy(point_shm_send->users[0], buf_name);
    status = semop(id_sem, &lock_2, 1);
        error_func(status);

    werase(wnd_send_sms);
    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
    wrefresh(wnd_send_sms);

    //- у нас будут два процесса: строко, которая посылает сообщения
    pid = fork();
        error_func(pid);

    if(0 == pid){

        while(1){
            sleep(1);

            //- обработка строки
            wgetnstr(wnd_send_sms, buf_sms, 28);
            //- стираем все из строки, куда вводили
            werase(wnd_send_sms);
            start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
            wrefresh(wnd_send_sms);

            status = semop(id_sem, lock_1, 2);
                error_func(status);

            if(0 == strcmp(buf_sms, "exit")){
                point_shm_send->type_sms = 1;
                strcpy(point_shm_send->users[0], buf_name);
                status = semop(id_sem, &lock_2, 1);
                    error_func(status);
                break;
            } else if(0 == strcmp(buf_sms, "fin")){
                point_shm_send->type_sms = 6;
                strcpy(point_shm_send->users[0], buf_name);
                status = semop(id_sem, &lock_2, 1);
                    error_func(status);
                break;
            } else {
                point_shm_send->type_sms = 3;
                strcpy(point_shm_send->users[0], buf_name);
                strcpy(point_shm_send->sms, buf_sms);
                status = semop(id_sem, &lock_2, 1);
                    error_func(status);
            } 

            werase(wnd_send_sms);
            start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
            wrefresh(wnd_send_sms);
        }

    } else if(0 < pid) {


        //- запоминаем начальные координаты, чтобы двигаться
        x_sms = 1;
        y_sms = 4;
        //- убираем лишнюю информацию в окнах;
        werase(wnd_urers);
        werase(wnd_see_sms);
        start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
        wrefresh(wnd_see_sms);
        //- входим в цикл, когда нам сервер предоставит новые данные, либо отключит от сессии
        while(1){
            sleep(1);
        
            status = semop(id_sem, &lock_3, 1);
                error_func(status);

            if(-4 == point_shm_get->type_sms){

                //- ищем пользователя на сервере;
                for(i = 0; i < 10; i++){
                    if(!strcmp(buf_name, point_shm_get->users[i])){
                        yes_or_no = 1;
                    }
                }
                if(1 == yes_or_no){
                    continue;
                } else {
                    printf("Сервер переполнен или закрыт\n");
                    break;
                }

            } else if(4 == point_shm_get->type_sms){
                //- тут обрабатываем окно с пользователями;
                werase(wnd_urers);
                start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
                for(i = 0; i < point_shm_get->amount_users; i++){
                    wmove(wnd_urers, 4 + i, 1);
                    wprintw(wnd_urers,"%s", point_shm_get->users[i]);
                    wrefresh(wnd_urers);
                }

            } else if(5 == point_shm_get->type_sms){

                //- тут обрабатываем окно с смс
                if(MAX_SMS + 4 == y_sms){
                    werase(wnd_see_sms);
                    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
                    x_sms = 1;
                    y_sms = 4;
                }

                wmove(wnd_see_sms, y_sms, x_sms + 1);
                wprintw(wnd_see_sms,"%s", point_shm_get->sms);
                wrefresh(wnd_see_sms);

                y_sms++;
            }  else if(-1 == point_shm_get->type_sms){

                //- сообщяем, что услышали о завершении сеанса;
                status = semop(id_sem, &lock_4, 1);
                    error_func(status);
                status = semop(id_sem, &lock_5, 1);
                    error_func(status);

                break;
            }

            wmove(wnd_send_sms, 1, 1);
            wrefresh(wnd_send_sms);

            //- сообщяем, что прочитали сообщение и хотим идти дальше;
            status = semop(id_sem, &lock_4, 1);
                error_func(status);
            status = semop(id_sem, &lock_5, 1);
                error_func(status);
        }

        pid = wait(&status);
            error_func(pid);
        if(!WIFEXITED(status)){
            printf("Дочерний процесс (1) завершился некорректно");
        }

        delwin(wnd_send_sms);
        delwin(wnd_urers);
        delwin(wnd_see_sms);

        endwin();   

    }

     exit(EXIT_SUCCESS);
}

void start_logo(WINDOW *wnd_see_sms, WINDOW *wnd_urers, WINDOW *wnd_send_sms){
    //- обрамляем окно wnd символами;
    box(wnd_send_sms, '|', '-');
    box(wnd_urers, '|', '-');
    box(wnd_see_sms, '|', '-');

    //- делаем подписи для подокон
    wmove(wnd_see_sms, 1, 2);
    wprintw(wnd_see_sms, "SMS");
    wmove(wnd_see_sms, 2, 2);
    wprintw(wnd_see_sms, "*****");
    wmove(wnd_urers, 1, 2);
    wprintw(wnd_urers, "Users");
    wmove(wnd_urers, 2, 2);
    wprintw(wnd_urers, "*****");
}