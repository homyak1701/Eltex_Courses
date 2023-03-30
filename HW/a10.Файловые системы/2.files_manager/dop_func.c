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
void coloring(WINDOW *wnd_color, int put, int x, int y, struct dirent *namelist);
void put(WINDOW *wnd_names, WINDOW *wnd_sizes, WINDOW *wnd_mod, WINDOW *wnd, int put, struct dirent *namelist, int num_files);
void coloring(WINDOW *wnd_color, int x, int y, struct dirent *namelist);

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

    while(num_files--){
        wmove(wnd_names, shift_y, 3);
        wmove(wnd_sizes, shift_y, 3);
        wmove(wnd_mod, shift_y, 3);
        if(DT_DIR == namelist[num_files]->d_type){
            wprintw(wnd_names, "/%s", namelist[num_files]->d_name);
        } else {
            wprintw(wnd_names, "%s", namelist[num_files]->d_name);
        }
        result = stat(namelist[num_files]->d_name, &statbuf);
            error_stat(result);
        wprintw(wnd_sizes, "%d", statbuf.st_size);
        strftime(buf_time, sizeof(buf_time), "%d.%m.%Y %H:%M:%S", localtime(&statbuf.st_ctime));
        wprintw(wnd_mod, "%s", buf_time);
        shift_y++;
    }

    wrefresh(wnd);
}

//- сместить курсор;
void put(
            WINDOW *wnd_names, 
            WINDOW *wnd_sizes, 
            WINDOW *wnd_mod, 
            WINDOW *wnd, 
            int put,
            struct dirent *namelist,
            int num_files
        )
{
    //- текущие координаты в окне;
    int x = 0, y = 0;
    
    getyx(wnd, y, x);
    wmove(wnd_names, y, x);
    wmove(wnd_sizes, y, x);
    wmove(wnd_mod, y, x);

    if(((y + put) >= 3) && ((y + put) <= (num_files + 3))){
        coloring(wnd_names, x, y + put, namelist);    
        coloring(wnd_sizes, x, y + put, namelist);
        coloring(wnd_mod, x, y + put, namelist);
    }
}

//- закрасить нужную область;
void coloring(
                WINDOW *wnd_color,
                int x,
                int y,
                struct dirent *namelist
            )
{
    //- для размера окна;
    int subwin_y = 0, subwin_x = 0;
    //- создадим подокно для отображения выделенного файла;
    WINDOW *wnd_file;

    getmaxyx(wnd_color, subwin_y, subwin_x);
    wmove(wnd_color, y, x);
    wnd_file = derwin(wnd_color, 1, subwin_x, y, x);

    wattron(wnd_file, COLOR_PAIR(1));
    wbkgd(wnd_file, COLOR_PAIR(1));

    wattron(wnd_file, A_REVERSE);



}

//- продолжить строка 94;


