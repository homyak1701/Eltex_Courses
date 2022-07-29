//компиляция gcc -lncurses

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <stdlib.h>
#include <curses.h>

/*функция оброботчика сигнала SIGWITCH*/

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);//- функция для получания 
							  //новых значений экрана
	/*1) дескриптор файла устройства предоставляющий терминал;
	  2) константа;
	  3) адрес структуры winsize;*/
	resizeterm(size.ws_row, size.ws_col); //- аргументами функции обработки экрана
					      //служат новые параметры экрана в строках
					      //и столбцах
}

/*
int main(int argc, char ** argv)
{
	initscr(); //- инициализация структур данных ncurses;
		   //- переводит терминал в нужных режим;

	//- SIGWITCH - сигнал, который возникает при изменении
	//параметров окна терминала

	signal(SIGWINCH, sig_winch); //- обработка сигнала SIGWITCH 
	cbreak();

	noecho();//- отключает отображение символов, вводимых с
		 //клавиатуры;
		 
	curs_set(1);//- функция для управления видимостью
		    //курсора (в нашес случае невидимый);
	attron(A_BOLD); //- включает атрибуты текста, которые применяются 
			//после вызова функции;
			//- A_UNDERLINE - подчеркивание;
			//- A_REVERSE - меняетя цвет фона и текста;
			//- A_BLINK - телает цвет мигающим;
			//- A_DIM - снижает яркость текста;
			//- A_BOLD - делает текст жирным;
			//- COLOR_PAIR() применяется для установки 
			//цветов фона и текста.
	move(5, 20); //- устанавливает позицию курсора в окне stdscr;
	printw("Hello, brave new curses world!\n"); //вывод строки на экран;
	attroff(A_BOLD); //сбросить атрибуты текста 
	attron(A_BLINK);
	move(7, 21);
	printw("Press any key to continue...");
	refresh(); //- отображение напечатанных символов;
	getch(); //- сичтывание символов из потока ввода терминала;
	endwin(); //- восстанавливает то состояние, в котором
		  //терминал находился до инициализации ncurses;
	exit(EXIT_SUCCESS);
}
*/

// до этого все функции предназначены для работы с stdscr;

//при работе в окне мы используем префикс w;
int main(int argc, char ** argv)
{
	WINDOW * wnd; //- указатель на окно
	WINDOW * subwnd; //- указатель на подокно

	initscr();
	signal(SIGWINCH, sig_winch);
	cbreak();
	curs_set(0);
	refresh();
	wnd = newwin(6, 18, 2, 4);
	box(wnd, '|', '-');
	subwnd = derwin(wnd, 4, 16, 1, 1);
	wprintw(subwnd, "Hello, brave new curses world!\n");
	wrefresh(wnd);
	delwin(subwnd);
	delwin(wnd);
	move(9, 0);
	printw("Press any key to continue...");
	refresh();
	getch();
	endwin();
	exit(EXIT_SUCCESS);

}
