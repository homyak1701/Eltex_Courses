/*командный интерпритатор*/


#include <stdio.h>
#include <errno.h> //значнеие errno зависит от вызываемых функций
		   // -1 функция выполнилась не правильно
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>

#define DEFAULT_BUFF_SIZE 16
#define TOKENS_DELIMITERS " \t"

/*прототипы функций*/
char *readline();
char **split_str(char* line);
char **split_tokens(char *str_process);

/*		    		*/
/*реализация функций*/
/*		    		*/

/*фнукция считывания строки из стандартного потока ввода*/

char *readline(){
	char* line = NULL; /*буфер для стандартного потока ввода*/
	size_t size = 0;
	ssize_t str_len; /*длина считанной строки*/

	/*Считываение строки из стандандартного потока ввода*/
	if(-1 == (str_len = getline(&line, &size, stdin))){
		if(-1 == errno){
			printf("[ERROR] Couuldn't read from stdin");
		}
		free(line); /*обнулим буфер, если ничего не вводилось*/
		printf("\n");
		return NULL;
	}
	
	/*убирает перенос на новую стровку в конце*/

	if ('\n' == line[str_len - 1]){
		line[str_len - 1] = '\0';
	}

	return line;
}

/*из введенной строки выбираем токины и записываем в массив*/

char** split_str(char* line){

	size_t buf_size = DEFAULT_BUFF_SIZE; /*изначальный размер буфера для токенов*/
	size_t point = 0; /*счетчик токинов*/
	char *sep = "|";	
	char *str = NULL; 

	char **str_process = (char **)malloc(sizeof(char *) * DEFAULT_BUFF_SIZE);
	if (str_process == NULL){
		printf("[ERROR] Couldn't allocated buffer for spliting");
		return NULL;
	}
	/*функция для разделение токенов*/
	str = strtok(line, sep);

	while(0 != str){

		str_process[point] = str;

		/*если буфер оказался маленьким, то увеличим его*/
		if(point >= buf_size){
			buf_size *= 2;
			str_process = (char**)realloc(str_process, buf_size * sizeof(char *));
			if(str_process == NULL){
				printf("[ERROR] Couldn't reallocate buffer for str_process\n");
				return NULL;
			}
		}
		/*берем следующий токен*/
		str = strtok(NULL, sep);
		point++;
	}
	str_process[point] = NULL; /*в конец токенов добавляем NULL, так как функция exec() 
	ожидает наличие данного завершающего токена*/
	return str_process;
}

char **split_tokens(char *str_process){

	size_t buf_size = DEFAULT_BUFF_SIZE; /*изначальный размер буфера для токенов*/
	size_t point = 0; /*счетчик токинов*/
	char *sep = " ";	
	char *str = NULL; 

	char **tokens = (char **)malloc(sizeof(char *) * DEFAULT_BUFF_SIZE);
	if (tokens == NULL){
		printf("[ERROR] Couldn't allocated buffer for spliting");
		return NULL;
	}
	/*функция для разделение токенов*/
	str = strtok(str_process,sep);

	while(0 != str){

		tokens[point] = str;

		/*если буфер оказался маленьким, то увеличим его*/
		if(point >= buf_size){
			buf_size *= 2;
			tokens = (char**)realloc(tokens, buf_size * sizeof(char *));
			if(tokens == NULL){
				printf("[ERROR] Couldn't reallocate buffer for tokens\n");
				return NULL;
			}
		}
		/*берем следующий токен*/
		str = strtok(NULL, sep);
		point++;
	}
	tokens[point] = NULL; /*в конец токенов добавляем NULL, так как функция exec() 
	ожидает наличие данного завершающего токена*/
	return tokens;
}


int main (int argc, char **argV)
{
	system("clear");

	pid_t childPid = 0;/*переменная для разделения родительского и дочернего процессов*/
	pid_t w;
	int nArgc = 0;
    int point1 = 0, point2 = 1; /*перечисление процессов для каналов для родителя и потомка*/
	int amount_bytes = 0;/*количетсво байт, которые будут записываться в */
	int errorExecv;
	int status; /*для wait*/
    int read_child[2], write_child[2]; /*создание массива для хранения массива дескриптора канала*/
	int fd_back_child[2], fd_back_parent[2];
	char **str_process = NULL;
	char **tokens = NULL;
	char *string_comands = NULL; /*переменная для чтения из стандартного потока ввода и из буфера канала*/

	printf("\nEnter your comanl --> ");
	string_comands = readline();
		
	str_process = split_str(string_comands);

        if(-1 == (pipe(read_child))){
            perror("pipe_create");
            exit(EXIT_FAILURE);
        }

		if(-1 == pipe(write_child)){
			perror("pipe_create");
			exit(EXIT_FAILURE);
		}

		childPid = fork();
		if(-1 == childPid){
			perror("Error fork()");
			exit(EXIT_FAILURE);
		}


            if(0 == childPid){  /*дочерний процесс*/
				close(read_child[1]); /*закрываем ненужные дескрипторы канала*/
				close(write_child[0]);

				fd_back_child[0] = dup(STDIN_FILENO); /*дескрипторы стандартногоптока записываем*/
				fd_back_child[1] = dup(STDOUT_FILENO); /*чтобы в конце дескрипторы вернуть назад*/

				dup2(read_child[0], STDIN_FILENO); /*перенапрвляем дескрипторы на буфер канала*/
				dup2(write_child[1], STDOUT_FILENO);

				while(0 != str_process[point2]){ /*дочерний процесс будет чуществовать
													пока не закончатся выбранные процессы пользователем*/
													
					tokens = split_tokens(str_process[point2]);
					if(0 != str_process[point2 + 1]){
						point2 = point2 + 2;
					} else {
						close(write_child[1]);
						write_child[1] = -1;
						dup2(fd_back_child[1], STDOUT_FILENO);
					}
					
					errorExecv = execvp(tokens[0], tokens); /*начинаем выолняем команду после |*/
					if (errorExecv < 0){
						perror("Error");
						exit(EXIT_FAILURE);
					}
				}
				
				if (0 > close(read_child[0])){
						perror("Error_close_read_child[0]");
						exit(EXIT_FAILURE);
				}
				if(write_child[1] > 0){
					if (0 > close(write_child[1])){
							perror("Error_close_read_child[1]");
							exit(EXIT_FAILURE);
					}
				}
				dup2(fd_back_child[0], STDIN_FILENO);
				dup2(fd_back_child[1], STDOUT_FILENO);

				close(fd_back_child[0]);
				close(fd_back_child[1]);
				
            } else { /*родительский процесс*/
                close(read_child[0]);
                close(write_child[1]);

				fd_back_parent[0] = dup(STDIN_FILENO);
				fd_back_parent[1] = dup(STDOUT_FILENO);

				dup2(read_child[1], STDOUT_FILENO);/*перенаправляем стандартный вывод в буфер канала*/
				dup2(write_child[0], STDIN_FILENO);

				while(0 != str_process[point1 + 1]){
	
					tokens = split_tokens(str_process[point1]); /*выделяем токены первого процесса*/
					point1 = point1 + 2; /*перескакиваем через один, то есть следующий процесс 
											родителя*/
					execvp(tokens[0],tokens); /*выполняем первый процесс до разделителя | */
					w = wait(&status); //ждем пока выполнится дочерний процесс
					if(-1 == w){
						perror("error childPid");
						exit(EXIT_FAILURE);
					}
				}
				
				if(0 != str_process[point1]){
					if (-1 == close(read_child[1])){
							perror("Error_close_write_child[1]");
							exit(EXIT_FAILURE);
					} 
					dup2(fd_back_parent[1], STDOUT_FILENO); /*в конце перенаправляем вывод на экран*/
					tokens = split_tokens(str_process[point1]);
					
					execvp(tokens[0],tokens);

					if (-1 == close(write_child[0])){
							perror("Error_close_write_child[0]");
							exit(EXIT_FAILURE);
					}
					dup2(fd_back_parent[0], STDIN_FILENO);
					
          		} else {
					if (-1 == close(write_child[0])){
							perror("Error_close_write_child[0]");
							exit(EXIT_FAILURE);
					}
					if (-1 == close(read_child[1])){
							perror("Error_close_write_child[1]");
							exit(EXIT_FAILURE);
					}
					
					dup2(fd_back_parent[1], STDOUT_FILENO);
					dup2(fd_back_parent[0], STDIN_FILENO);

				}
				close(fd_back_parent[1]);
				close(fd_back_parent[0]);
        	}
		free(str_process);
		free(tokens);	
	return 0;
}
