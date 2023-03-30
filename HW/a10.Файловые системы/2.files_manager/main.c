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

#include "dop_func.h"

void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}


int main(int argc, char* argV[]){

    //- для промежуточных значений при распределении подокон в окне;
    //- сколько пространтсва уже использовалось;
    int wnd_xs_use = 0;
    //- сколько пространтсва еще осталось;
    int wnd_xs_rm = 0;

    //- создадим указатели на окна;
    WINDOW *wnd_left;
    WINDOW *wnd_right;

    //- подокна с информацией для левого окна;
    WINDOW *wnd_left_names;
    WINDOW *wnd_left_sizes;
    WINDOW *wnd_left_mod;

    //- подокна с информацией для правого окна;
    WINDOW *wnd_right_names;
    WINDOW *wnd_right_sizes;
    WINDOW *wnd_right_mod;

    //- значения координат;
    int x = 0, y = 0;
    //- метаданные каталога;
    struct dirent **namelist;
    //- количетсво файлов в директории;
    int num_files = 0;
    //- код клавиши;
    int put_code = 0;
    //- режим окна (правое/левое)
    char *mode_see = "left";

    //- получаем метаинформацию текущего каталога;
    num_files = scandir(".", &namelist, NULL, alphasort);
        error_dir(num_files);
    
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

    //- создаем цветовые пары;
    //- для фона и букв;
    init_pair(1, COLOR_WHITE, COLOR_BLUE);
    //- для фона и разделов;
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);

    bkgd(COLOR_PAIR(1));
    refresh();

    //- создаем два окна, как в mc;
    wnd_left = newwin((stdscr->_maxy) - 1, (stdscr->_maxx/2) - 1, 1, 1);
    wnd_right = newwin((stdscr->_maxy) - 1,(stdscr->_maxx/2) - 1, 1, (stdscr->_maxx/2));
    //- установим настройки цвета для двоих окон;
    wattron(wnd_left, COLOR_PAIR(1));
    wbkgd(wnd_left, COLOR_PAIR(1));

    wattron(wnd_right, COLOR_PAIR(1));
    wbkgd(wnd_right, COLOR_PAIR(1));

    //- обрамляем окно wnd символами;
    box(wnd_left, '|', '-');
    box(wnd_right, '|', '-');

    //- создаем подокна

    //- ниже для левого подокна;
    //- wnd_xs_use - сколько использовано столбцов;
    //- wnd_xs_rm - сколько осталось столбцов;
    wnd_xs_use = 0;
    wnd_xs_rm = stdscr->_maxx/2 - 1;

    wnd_left_names = derwin(wnd_left, (stdscr->_maxy) - 1, wnd_xs_rm * 0.66 + 1, 0, 0);
    wattron(wnd_left_names, COLOR_PAIR(1));
    wbkgd(wnd_left_names, COLOR_PAIR(1));
    box(wnd_left_names, '|', '-'); 

    wnd_xs_use = wnd_xs_use + (wnd_xs_rm * 0.66);
    wnd_xs_rm = wnd_xs_rm - (wnd_xs_rm * 0.66);

    wnd_left_sizes = derwin(wnd_left, (stdscr->_maxy) - 1, wnd_xs_rm * 0.33 + 1, 0, wnd_xs_use);
    wattron(wnd_left_sizes, COLOR_PAIR(1));
    wbkgd(wnd_left_sizes, COLOR_PAIR(1));
    box(wnd_left_sizes, '|', '-');

    wnd_xs_use = wnd_xs_use + (wnd_xs_rm * 0.33);
    wnd_xs_rm = wnd_xs_rm - (wnd_xs_rm * 0.33);

    wnd_left_mod =  derwin(wnd_left, (stdscr->_maxy) - 1, wnd_xs_rm + 1, 0, wnd_xs_use) ;
    wattron(wnd_left_mod, COLOR_PAIR(1));
    wbkgd(wnd_left_mod, COLOR_PAIR(1));
    box(wnd_left_mod, '|', '-');

    //- ниже для правого подокна;
    //- wnd_xs_use - сколько использовано столбцов;
    //- wnd_xs_rm - сколько осталось столбцов;
    wnd_xs_use = 0;
    wnd_xs_rm = stdscr->_maxx/2 - 1;

    wnd_right_names = derwin(wnd_right, (stdscr->_maxy) - 1, wnd_xs_rm * 0.66 + 1, 0, 0);
    wattron(wnd_right_names, COLOR_PAIR(1));
    wbkgd(wnd_right_names, COLOR_PAIR(1));
    box(wnd_right_names, '|', '-'); 

    wnd_xs_use = wnd_xs_use + (wnd_xs_rm * 0.66);
    wnd_xs_rm = wnd_xs_rm - (wnd_xs_rm * 0.66);

    wnd_right_sizes = derwin(wnd_right, (stdscr->_maxy) - 1, wnd_xs_rm * 0.33 + 1, 0, wnd_xs_use);
    wattron(wnd_right_sizes, COLOR_PAIR(1));
    wbkgd(wnd_right_sizes, COLOR_PAIR(1));
    box(wnd_right_sizes, '|', '-');

    wnd_xs_use = wnd_xs_use + (wnd_xs_rm * 0.33);
    wnd_xs_rm = wnd_xs_rm - (wnd_xs_rm * 0.33);

    wnd_right_mod =  derwin(wnd_right, (stdscr->_maxy) - 1, wnd_xs_rm + 1, 0, wnd_xs_use) ;
    wattron(wnd_right_mod, COLOR_PAIR(1));
    wbkgd(wnd_right_mod, COLOR_PAIR(1));
    box(wnd_right_mod, '|', '-');

    //- делаем подписи для подокон

    wmove(wnd_right_names, 1, 3);
    wprintw(wnd_right_names, "Names");
    wmove(wnd_right_sizes, 1, 2);
    wprintw(wnd_right_sizes, "Sizes");
    wmove(wnd_right_mod, 1, 2);
    wprintw(wnd_right_mod, "Modify time");

    wmove(wnd_left_names, 1, 3);
    wprintw(wnd_left_names, "Names");
    wmove(wnd_left_sizes, 1, 2);
    wprintw(wnd_left_sizes, "Sizes");
    wmove(wnd_left_mod, 1, 2);
    wprintw(wnd_left_mod, "Modify time");

    //- выведем имена всех файлов их метаданные;
    //- воспользуемся фукнциями, которые сами создали;
    see_wnd_dir(3, num_files, wnd_left_names, wnd_left_sizes, wnd_left_mod, wnd_left, namelist);
    see_wnd_dir(3, num_files, wnd_right_names, wnd_right_sizes, wnd_right_mod, wnd_right, namelist);

    //- смещаем курсор на начало;
    wmove(wnd_left, 3, 3);
    wmove(wnd_right, 3, 3);

    //- переходим в режим raw_mode, чтобы пользователю не нужно было каждый раз нажимать Enter;
    cbreak();

    //- включаем режим обработки функциональных клавиш и стрелок
    keypad(wnd_left, TRUE);
    keypad(wnd_right, TRUE);

    put_code = wgetch(wnd_left);

    //- заходим в бесконечный цикл пользователя;
    //- пока пользователь не нажмет Esc он будет находится в плиложении;
    while(27 != put_code){
        put(wnd_left_names, wnd_left_sizes, wnd_left_mod, wnd_left, 0, namelist[0], num_files);
        switch(put_code){
            //ствелочка вверх;
            case 259:
                if("left" == mode_see){
                    //put();
                    put_code = wgetch(wnd_left);
                } else if("right" == mode_see){
                    //put();
                    put_code = wgetch(wnd_right);
                }
                break;
            //стрелочка вниз;
            case 258:
                if("left" == mode_see){
                    put_code = wgetch(wnd_left);
                } else if("right" == mode_see){
                    put_code = wgetch(wnd_right);
                }
                break;
            //Enter
            case 10:
                if("left" == mode_see){
                    put_code = wgetch(wnd_left);
                } else if("right" == mode_see){
                    put_code = wgetch(wnd_right);
                }
                break;
            //TAB (Ctrl+I)
            case 9:
                if("left" == mode_see){
                    mode_see = "right";
                    put_code = wgetch(wnd_right);
                } else if("right" == mode_see){
                    mode_see = "left";
                    put_code = wgetch(wnd_left);
                }
                break;
            default:
                if("left" == mode_see){
                    put_code = wgetch(wnd_left);
                } else if("right" == mode_see){
                    put_code = wgetch(wnd_right);
                }
                break;
        }
    } 

    //- после окончания работы удаляем все окна и подокна

    delwin(wnd_left_names);
    delwin(wnd_left_sizes);
    delwin(wnd_left_mod);

    delwin(wnd_left);

    delwin(wnd_right_names);
    delwin(wnd_right_sizes);
    delwin(wnd_right_mod);

    delwin(wnd_right);

    //- восстанавливаем то состояние, в котором терминал находился до состояния ncurses();
    endwin();

    return 0;
}