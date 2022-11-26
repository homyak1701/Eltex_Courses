#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <malloc.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <signal.h>
#include <curses.h>

#include <dlfcn.h>
#include <sys/types.h>
#include <dirent.h>

#define N 60

int choiseFile(WINDOW * wnd);
int serchFile(struct dirent ***nameFile);
int openScreen(struct dirent ***pointNamelist, WINDOW * wnd_open, int n_files);
//int openFile(char *nameFile, char mode);
int readFile(char *buf_read, int fd);
//int writeFile(char *buf_write, int fd);

/*обработчик сигнала SIGWINCH*/

void sig_winch(int signo)
{
	struct winsize size;
	ioctl(fileno(stdout), TIOCGWINSZ, (char *) &size);
	resizeterm(size.ws_row, size.ws_col);
}

/*поиск файлов в директории files*/
/*возвращает количетсво найденных файлов*/

int choiseFile(WINDOW * wnd) /*wnd_open*/
{
	int get_char;
	
	int x = 0;
	int y = 1;
	wmove(wnd, 1, 0);
	wprintw(wnd,">");

	while ( get_char != '\n' )
	{
		noecho(); //убрать отображение на экран
		switch ( get_char = wgetch(wnd) )
		{
			case (int)KEY_UP:
				getyx(wnd,y,x);
				if(1 != y)
				{
					x--; /*координаты принимаются с увеличенным x*/
	                                wmove(wnd, y, x);
					wprintw(wnd," ");
					wmove(wnd, y-2, x);
					wprintw(wnd,">");
					refresh();
				}
				break;
				
			case (int)KEY_DOWN:
				getyx(wnd,y,x);
				if(5 != y)
				{
					x--;
	                                wmove(wnd, y, x);
					wprintw(wnd," ");
                	                wmove(wnd, y+2, x);
                        	        wprintw(wnd,">");
					refresh();
				}
				break;
				
			default: break;
		}
	}
	echo();
	getyx(wnd,y,x);
	return (y/2)+2;
}


/*поиск имен файлов в указанной директории*/
/*возвращает количетсво найденных файлов*/

int serchFile(struct dirent ***pointerNamelist)
{
	int countLibrary = scandir("./files", pointerNamelist, 0, alphasort);
	if (countLibrary < 0) 
	{
		perror("scandir");
		exit(-1);
	}

	return countLibrary;  
}

/*в начале работы программы и после каджого раза, как заканчивается редактирование файла эта функция 
вызывает окно главного меню*/

int openScreen(struct dirent ***pointNamelist, WINDOW * wnd_open, int n_files)
{

		int choise;
		struct dirent **nameList = *pointNamelist;

                for(int i = 2; i < n_files; i++) /*перечесление начинаем с 2 т.к. первые два файла это директории*/
		{
                        wprintw(wnd_open, "\n  %d) %s\n", i-1, nameList[i]->d_name);
                }
                
		wrefresh(wnd_open);
                wprintw(wnd_open, "\n\n--- Enter your file ---");
        	choise = choiseFile(wnd_open);
	        wrefresh(wnd_open);
	        sleep(1);
		
		return choise;
}


/*функция для открытия или создания файла*/
/*передается имя файла и в каком режиме его открывать (просто открыть или еще создать файл)*/

int openFile(char *nameFile, char mode)
{
	int fd;
	switch( mode )
	{
		case 'o':
			fd = open(nameFile, O_TRUNC);
			break;
		case 'c':
			fd = open(nameFile, O_CREAT | O_RDWR | O_TRUNC, S_IRWXU);
			break;
		if (-1 == fd)
		{
			perror("ошибка при открытии/создании файла");
			exit(1);
		}
	}
	if (-1 == fd)
        {
        	perror("ошибка при открытии/создании файла");
        	exit(1);
        }
	return fd;
}

int writeFile(char *buf_write, int fd)
{
	int amount_w;
	
	/*созраняем в память из буфера записи (buf_write)*/

	if(-1 == (amount_w = write(fd, buf_write, strlen(buf_write))))
        {
                perror("\nОшибка при записи");
                exit(1);
        }

	return amount_w;	
}

int readFile(char *buf_read, int fd)
{
	int amount_r;

	if(-1 == (amount_r = read(fd, buf_read, 1800)))
        {
                perror("\nОшибка при чтении");
                exit(1);
        }

	return amount_r;
}

int main(int argc, char ** argv)
{

//	char buf_write[1800]; /*файловый буфер для записи*/
//	memset(buf_write, ' ', 1800);
	char buf_read[1800]; /*файловый буфер для чтения*/
//	memset(buf_read, ' ', 1800);

	int i = 0; /*счетчик для  циклов*/
	char esc; /*переменная для выхода из программы*/ 
	int choise;/*перемнная для номера выбранного пользователем файла*/
	int amount_r = 0;
	int amount_w = 0;

	struct dirent **namelist = NULL; /*там лежат все названия файлов для пользования в main*/
	int n_files; /*количетсво наденных файлов*/

	WINDOW * wnd_open; /*окно для открытия файла*/
	WINDOW * subwnd; /*окно завершения работы*/
	WINDOW * wnd_write;/*окно для записи в файл или его изменения*/

	int fd;/*файловый дескриптор*/
	int x; /*ось x в ncurses или столбец*/
        int y; /*ось y в ncurses или строка*/
        int x_point = 1;/*счетчик записанных символов*/
        int get_char;/*принимаемый символ с клавиатуры от пользователя*/

	char nameFile[30] = {"./files/"};

	/*указатели для работы с функциями*/

	struct dirent ***pointer_namelist; /*указатель для записи названий файлов из директории*/
//	char *point_bufer_write = buf_write; /*указатель на файловый буфер write*/
	char *point_bufer_read = buf_read; /*указатель на файловый буфер read*/
	
	pointer_namelist = &namelist; /*принимает ссылку на массив, где лежат назания файлов директории*/
	n_files = serchFile(pointer_namelist);

	
	/**/			/**/
	/*начало работы с ncurses*/
	/**/			/**/


        initscr(); //начало визуализуции
        signal(SIGWINCH, sig_winch);
        start_color(); /*инициализация работы с цветом;*/
        refresh();     /*отображение сделанных настроек ранее*/
        init_pair(1, COLOR_YELLOW, COLOR_BLUE); //- инициализация цвета и фона текста с номером 1;
        init_pair(2, COLOR_YELLOW, COLOR_MAGENTA); //- инициализация цdета и фона текста с номером 2; 

	/*создание и оформление окон*/
	
	wnd_open = newwin(15, 30, 2, 2); //- создание окна;
	wnd_write = newwin(30, N, 2, 40);
        	
	wbkgd(wnd_open, COLOR_PAIR(1));
        wattron(wnd_open, A_BOLD);
	
	wbkgd(wnd_write, COLOR_PAIR(2));
        wattron(wnd_write, A_BOLD);
	
        keypad(wnd_open, TRUE); //отключить ввод ESC-последовательностей
	keypad(wnd_write, TRUE);
        
	do
	{
		curs_set(0); /*видимый курсор*/
		sleep(1);
//		esc = wgetch(wnd_open);
		wclear(wnd_write);
		wrefresh(wnd_write);
		wmove(wnd_open, 0, 0);
		/*отображение меню с файлами*/
		n_files = serchFile(pointer_namelist);
		choise = openScreen(pointer_namelist, wnd_open, n_files);

		for (i = 8; i<20; i++)
		{
		nameFile[i] = (namelist[choise]->d_name)[i-8];
		}
		
		fd = open(nameFile, O_RDWR);
		if (fd == -1)
		{
			perror("Нельзя открыть файл");
			exit(EXIT_FAILURE);
		}


		memset(buf_read, ' ', 1800);


		amount_r = lseek(fd, 0, SEEK_SET);
		if (amount_r < 0) 
		{
			perror("Error");
			exit(EXIT_FAILURE);
		}

		amount_r = readFile(point_bufer_read, fd);
		
		wprintw(wnd_write, buf_read);
		wrefresh(wnd_write);

		/*getxy(wnd,y,x) - в переменные x, y - возвращает координаты курсора, при этом x будет увеличен на один*/

		curs_set(1);	
		
		x_point = 0;
		wmove(wnd_write, 0, 0);
		while( x_point < 1000)
		{
			switch ( get_char = wgetch(wnd_write) )
			{
				case '\e':
					x_point = 1000;
					if(-1 == (amount_w = write(fd, buf_read, strlen(buf_read))))
					{
						perror("Error_Write");
						exit(EXIT_FAILURE);
					}
					sync(); 
					close(fd);
					break;
				case (int)KEY_BACKSPACE:
					getyx(wnd_write, y, x);
					mvwaddch(wnd_write, y, x, ' ');
					wmove(wnd_write, y, x);
					wrefresh(wnd_write);
					break;
				case (int)KEY_RIGHT: //
					getyx(wnd_write, y, x);
					wmove(wnd_write, y, x+1);
					wrefresh(wnd_write);
					break;
				case (int)KEY_LEFT:
					getyx(wnd_write, y, x);
					wmove(wnd_write, y, x-1);
					wrefresh(wnd_write);
					break;
				case (int)KEY_UP:
					getyx(wnd_write, y, x);
					wmove(wnd_write, y-1,x);
					wrefresh(wnd_write);
					break;
				case (int)KEY_DOWN:
					getyx(wnd_write, y, x);
					wmove(wnd_write, y+1,x);
					wrefresh(wnd_write);
					break;
				default:
					getyx(wnd_write, y, x);
					wmove(wnd_write, y, x);
					/*превращаем двумерную область в одномерный массив и записываем туда символ*/
					buf_read[(y*(30))+x] = get_char; 
					wrefresh(wnd_write);
					break;
			}
		}

		esc = wgetch(wnd_write);

	} while ( '\e' != esc );
	
//	wrefresh(wnd_write);
        delwin(wnd_open);
//	delwin(wnd_read);
        //убираем курсор
        curs_set(FALSE);
        move(19, 4);
        printw("Press any key to continue...");
        refresh();
        getch();
        endwin();
        exit(EXIT_SUCCESS);
}
