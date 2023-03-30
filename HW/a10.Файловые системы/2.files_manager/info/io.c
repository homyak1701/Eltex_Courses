#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

#define MAX_NAME_LEN 15

void sig_winch(int signo){
    struct winsize size;
    ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
    resizeterm(size.ws_row, size.ws_col);
}

int main(){
    WINDOW *wnd;
    char name[MAX_NAME_LEN + 1];

    initscr();
    signal(SIGWINCH, sig_winch);
    curs_set(TRUE);
    start_color();
    refresh();

    init_pair(1, COLOR_YELLOW, COLOR_BLUE);
    wnd = newwin(5, 23, 2, 2);
    wbkgd(wnd, COLOR_PAIR(1));
    wattron(wnd, A_BOLD);
    wprintw(wnd, "Enter your name...\n");
    wgetnstr(wnd, name, MAX_NAME_LEN);
    name[MAX_NAME_LEN] = 0;
    wprintw(wnd, "Hello, %s!", name);
    
    wrefresh(wnd);
    delwin(wnd);
    curs_set(FALSE);
    move(8, 4);
    printw("Press any key to continue...");
    refresh();
    getch();
    endwin();

    exit(EXIT_SUCCESS);
}

