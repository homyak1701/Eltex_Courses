#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

#include <dirent.h>

#include <stdio.h>
#include <string.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#include <unistd.h>

#define error_stat(a) do{if(a != 0){printf("line:%d\n", __LINE__); \
                                        perror("stat"); exit(EXIT_FAILURE);}} while(0)

//- прототипы функций
void see_wnd_dir(int shift_y, int num_files, WINDOW *wnd_names, WINDOW *wnd_sizes, WINDOW *wnd_mod, WINDOW *wnd, struct dirent **namelist);
void shift_cursor(WINDOW *wnd_names, WINDOW *wnd_sizes, WINDOW *wnd_mod, WINDOW *wnd, int put, struct dirent **namelist, int num_files);
void coloring(WINDOW *wnd_color, int x, int y, struct dirent **namelist, char *input);


//- вывести отображение всего окна;
void see_wnd_dir(
                int shift_y, 
                int num_files, 
                WINDOW *wnd_names, 
                WINDOW *wnd_sizes, 
                WINDOW *wnd_mod, 
                WINDOW *wnd, 
                struct dirent **namelist
            )
{
    //- проверка на получение метаданных;
    int result = 0;
    //- для хранения метаданных файла;
    struct stat statbuf;
    //- переменная, которая хранит дату последнего изменения файла
    char buf_time[80];
    // - для правильного отображения и правильного использования функции stat();
    char str_name[PATH_MAX];
    //- переменные ниже будут хранить размеры окна wnd_color;
    int subwin_y = 0, subwin_x = 0;
    //- счетчики;
    int i = 0, j = 0;

    //- узнаем размеры окна(подокна) и закрашиваем всю обрасть в синий;
    getmaxyx(wnd_names, subwin_y, subwin_x);
    for(i = 1; i < subwin_y-1; i++)
        for(int j = 1; j < subwin_x - 1; j++){
            wmove(wnd_names, i, j);
            wprintw(wnd_names, " ");
        }
    getmaxyx(wnd_sizes, subwin_y, subwin_x);
    for(i = 1; i < subwin_y - 1; i++)
        for(int j = 1; j < subwin_x - 1; j++){
            wmove(wnd_sizes, i, j);
            wprintw(wnd_sizes, " ");
        }
    getmaxyx(wnd_mod, subwin_y, subwin_x);
    for(i = 1; i < subwin_y - 1; i++)
        for(int j = 1; j < subwin_x - 1; j++){
            wmove(wnd_mod, i, j);
            wprintw(wnd_mod, " ");
        }

    //- делаем подписи для подокон
    wmove(wnd_names, 1, 3);
    wprintw(wnd_names, "Names");
    wmove(wnd_sizes, 1, 2);
    wprintw(wnd_sizes, "Sizes");
    wmove(wnd_mod, 1, 2);
    wprintw(wnd_mod, "Modify time");

    wmove(wnd, 3, 2);

    wbkgd(wnd_names, COLOR_PAIR(1));
    wbkgd(wnd_sizes, COLOR_PAIR(1));
    wbkgd(wnd_mod, COLOR_PAIR(1));

    for(i = 0; i < num_files; i++){
        wmove(wnd_names, shift_y, 2);
        wmove(wnd_sizes, shift_y, 2);
        wmove(wnd_mod, shift_y, 2);
        if(DT_DIR == namelist[i]->d_type){
            wprintw(wnd_names, "/%s", namelist[i]->d_name);
        } else {
            wprintw(wnd_names, "%s", namelist[i]->d_name);
        }

        result = stat(namelist[i]->d_name, &statbuf);
            error_stat(result);
        wprintw(wnd_sizes, "%d",(int)statbuf.st_size);
        
        strftime(buf_time, sizeof(buf_time), "%d.%m.%Y %H:%M", localtime(&statbuf.st_ctime));
        wprintw(wnd_mod, "%s", buf_time);
        shift_y++;
    }

    wrefresh(wnd_names);
    wrefresh(wnd_sizes);
    wrefresh(wnd_mod);
}

//- сместить курсор;
void shift_cursor(
            WINDOW *wnd_names, 
            WINDOW *wnd_sizes, 
            WINDOW *wnd_mod, 
            WINDOW *wnd, 
            int put,
            struct dirent **namelist,
            int num_files
        )
{
    //- текущие координаты в окне;
    int x = 0, y = 0;
    char str[50] = "/";
    char* str_name;
    //- проверка на получение метаданных;
    int result = 0;
    //- для хранения метаданных файла;
    struct stat statbuf;
    //- переменная, которая хранит дату последнего изменения файла
    char buf_time[80];

    getyx(wnd, y, x);

    //- проверка выхода за границы количетсва файлов
    if(((y + put) >= 3) && ((y + put) <= (num_files - 1 + 3))){

        //- затрем предыдущую раскраску;
        see_wnd_dir(3, num_files, wnd_names, wnd_sizes, wnd_mod, wnd, namelist);

        //- пока мы убирали закраску, коотординаты сместились, поэтому нужно их вернуть на место;
        wmove(wnd, y + put, x);
        wmove(wnd_mod, y + put, x);
        wmove(wnd_sizes, y + put, x);
        wmove(wnd_names, y + put, x);
        y = y + put;

        coloring(wnd_names, x, y, namelist, namelist[y - 3]->d_name);

        if(DT_DIR == namelist[y - 3]->d_type){
            str_name = namelist[y - 3]->d_name;
            strcat(str, str_name);
            coloring(wnd_names, x + 1, y, namelist, str);
        } else {
            coloring(wnd_names, x + 1, y, namelist, namelist[y - 3]->d_name);
        }

        result = stat(namelist[y - 3]->d_name, &statbuf);
            error_stat(result);

        sprintf(str, "%d", (int)statbuf.st_size);
        coloring(wnd_sizes, x, y, namelist, str);

        strftime(buf_time, sizeof(buf_time), "%d.%m.%Y %H:%M", localtime(&statbuf.st_ctime));
        coloring(wnd_mod, x, y, namelist, buf_time);
    }
}

//- закрасить нужную область;
void coloring(
                WINDOW *wnd_color,
                int x,
                int y, 
                struct dirent **namelist,
                char *input
            )
{
    //- переменные ниже будут хранить размеры окна wnd_color;
    int subwin_y = 0, subwin_x = 0;
    //- создадим указатель на подокно для отображения выделенного файла;
    WINDOW *wnd_file;
    //- узнаем размеры окна(подокна)
    getmaxyx(wnd_color, subwin_y, subwin_x);
    //- создадим подокно, что закрасит нужную область
    wnd_file = derwin(wnd_color, 1, subwin_x - 3, y, 2);
    if(NULL == wnd_file){
        perror("derwin");
        exit(EXIT_FAILURE);
    }

    //- Выбираем цвет в который хотим закрасить;
    wbkgd(wnd_file, COLOR_PAIR(2));
    wattron(wnd_file, A_REVERSE);

    //- Выводим информацию о файле(директории) в области закраски;
    wprintw(wnd_file, "%s", input);

    wrefresh(wnd_file);

    delwin(wnd_file);
}



