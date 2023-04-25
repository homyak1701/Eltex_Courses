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
#include <curses.h>
#include <dirent.h>
#include <time.h>
#include <malloc.h>

//- функция, что делает дефолтные надписи в окнах;
void start_logo(WINDOW *wnd_see_sms, WINDOW *wnd_urers, WINDOW *wnd_send_sms);

#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)


#define PRIOR_SEND_QUERTY_OR_SMS 5 //- приоритет, по которому отправляются запросы на сервер;
#define PRIOR_RECEPTION 6 //- приоритет, по которому мы принимает ответ от сервера;
#define MAX_USERS 10//- максимальное количетсво пользователей в сессии;
#define MAX_SMS 40 //- максимальное количетсво смс на экране;


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

    //- счетчик для циклов
    int i;

    //- код клавиши;
    int put_code = 0;

    key_t key_sms;
    //- для идентификатора очереди сообщения;
    int id_sms;
    //- структура, которую будем передавать, как сообщение
    struct send_and_request send_and_request;

    //- название файла;
    char *name_file = "file_key.txt"; 
    //- для создания нового процесса;
    //- для разделения поля имен и сообщений;
    pid_t pid;
    //- для проверки завершения дочернего процесса;
    int status;
    ssize_t status_rcv;

    key_sms = ftok(name_file, 6);    
        error_func(key_sms);

    id_sms = msgget(key_sms, IPC_CREAT | 0666);
        error_func(id_sms);   


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
    wgetnstr(wnd_send_sms, send_and_request.users[0], sizeof(send_and_request.users[0]));
    //- после того, как пользователь что-то отправил сотрем все ненужно в поле;
    werase(wnd_send_sms);
    werase(wnd_urers);
    werase(wnd_see_sms);
    //- и обновим названия полей;
    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);  

    id_sms = msgget(key_sms, IPC_EXCL | 0666);
    error_func(id_sms); 

    //- отправляем серверу запрос на вход в сессию
    send_and_request.mtype = PRIOR_SEND_QUERTY_OR_SMS;
    send_and_request.type_sms = 0;
    status = msgsnd(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), 0);
        error_func(status);

    werase(wnd_send_sms);
    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);

    //- ждем ответ от сервера, чтобы он нас подключил к сессии и предоставил имена всех пользователей;
    //printf("(1)Блокируюсь перед ответом на сервер, что можно войти\n");
    status_rcv = msgrcv(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), PRIOR_RECEPTION, 0);
    //printf("(1)Разблокироваля\n");
        error_func(status_rcv);
    //- проверяем подключились ли мы к серверу;
    if(-1 == send_and_request.priority){
        //- выходим из приложния, если сервер переполнен (да, жестокая реальность);
        printf("Сервер переполнен\n");
        kill(pid, SIGTERM);
        delwin(wnd_send_sms);
        delwin(wnd_urers);
        delwin(wnd_see_sms);

        endwin();
        exit(EXIT_SUCCESS);
    }

    //- выведем приходящую информацию и сессии
    for(i = 0; i < send_and_request.amount_users; i++){
        wmove(wnd_urers, 4 + i, 1);
        wprintw(wnd_urers,"%s", send_and_request.users[i]);
        wrefresh(wnd_urers);
    }

    //- у нас будут два процесса: строко, которая посылает сообщения
    pid = fork();
        error_func(pid);

    if(0 == pid){

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
            status_rcv = msgrcv(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), send_and_request.priority, 0);
                    error_func(status_rcv);
                
            if(-1 == send_and_request.amount_users){
                break;
            } else if(4 == send_and_request.type_sms){
                //- тут обрабатываем окно с пользователями;
                werase(wnd_urers);
                start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
                for(i = 0; i < send_and_request.amount_users; i++){
                    wmove(wnd_urers, 4 + i, 1);
                    wprintw(wnd_urers,"%s", send_and_request.users[i]);
                    wrefresh(wnd_urers);
                }

            } else if(5 == send_and_request.type_sms){

                //- тут обрабатываем окно с смс
                if(MAX_SMS + 4 == y_sms){
                    werase(wnd_see_sms);
                    start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
                    x_sms = 1;
                    y_sms = 4;
                }

                wmove(wnd_see_sms, y_sms, x_sms + 1);
                wprintw(wnd_see_sms,"%s", send_and_request.sms);
                wrefresh(wnd_see_sms);

                y_sms++;
            }    

            wmove(wnd_send_sms, 1, 1);
            wrefresh(wnd_send_sms);

        }

    } else if(0 < pid) {

        send_and_request.mtype = PRIOR_SEND_QUERTY_OR_SMS;
        while(1){
            sleep(1);
            wmove(wnd_send_sms, 1, 1);

            //- обработка строки
            wgetnstr(wnd_send_sms, send_and_request.sms, 28);
            werase(wnd_send_sms);
            start_logo(wnd_see_sms, wnd_urers, wnd_send_sms);
            wrefresh(wnd_send_sms);

            if(0 == strcmp(send_and_request.sms, "exit")){
                send_and_request.type_sms = 1;
                status = msgsnd(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), 0);
                    error_func(status);
                break;
            } else if(0 == strcmp(send_and_request.sms, "fin")){
                send_and_request.type_sms = 6;
                status = msgsnd(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), 0);
                    error_func(status);
                break;
            } else {
                send_and_request.type_sms = 3;
                status = msgsnd(id_sms, &send_and_request, sizeof(send_and_request) - sizeof(send_and_request.mtype), 0);
                    error_func(status); 
            }  
            
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

        //printf("Блольше я не должен блокироваться\n");
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