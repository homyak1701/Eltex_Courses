#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

//- функция по работе с динамическим окном;
void sig_winch(int signo){
    struct winsize size;
    //- получаем новые размеры окна;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *)&size);
    //- изменяем текущие размеры окна;
    resizeterm(size.ws_row, size.ws_col);
}


int main(int argc, char* argV[]){

    //- создадим указатели на окна;
    WINDOW *wnd;
    WINDOW *subwnd;

    //- инициализирем структуры данных ncurses и переводим терминал в нужный режим;
    initscr();
    //- устанавливаем обработчик сигнала SIGWINCH;
    signal(SIGWINCH, sig_winch);
    //- отключаем отображение символов, вводимых с клавиатуры;
    noecho();
    //- инициализируем управлением цветом;
    start_color();
    //- отключаем отображение курсора;
    curs_set(0);
    //- применим отображение на экран;
    refresh();

    //- создаем цветовые пары
    init_pair(1, COLOR_BLUE, COLOR_GREEN);
    init_pair(2, COLOR_YELLOW, COLOR_BLUE);

    //- создаем окно;
    wnd = newwin(6, 18, 2, 4);
    //- установим настройки цвета для символов в внешнего окна;
    wattron(wnd, COLOR_PAIR(1));
    //- обрамляем окно wnd символами;
    box(wnd, '|', '-');
    //- создаем подокно;
    subwnd = derwin(wnd, 4, 16, 1, 1);
    //- устанавливаем настройки цвета для фона в подокне;
    wbkgd(subwnd, COLOR_PAIR(2));

    //- делаем текст жирным в подокне;
    wattron(subwnd, A_BOLD | );
    wprintw(subwnd, "Hello World!\n");
    wrefresh(subwnd);
    wrefresh(wnd);

    delwin(subwnd);
    delwin(wnd);

    //- делаем текст мигающим;
    attron(A_BLINK);
    move(9, 0);
    printw("Press any key to continue...");
    
    //- применим отображение на экран;
    refresh();
    //- чтобы завержить программу добавим считывание с клавиатуры;
    getch();

    //- восстанавливаем то состояние, в котором терминал находился до состояния ncurses();
    endwin();

    return 0;
}