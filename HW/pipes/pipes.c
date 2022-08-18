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

/*		    */
/*реализация функций*/
/*		    */

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

	pid_t childPid = 0;
	pid_t w;
	int nArgc = 0;
    int i = -1, point = 0;
	int errorExecv;
	int status; /*для wait*/
    int fildes[2]; /*создание массива для хранения массива дескриптора канала*/
	char **str_process = NULL;
	 char **tokens = NULL;
	char *string_comands = NULL;
		
	printf("\nEnter your comanl --> ");
	string_comands = readline();
		
	str_process = split(string_comands);

        if(-1 == (pipe(fildes))){
            perror("pipe_create");
            exit(EXIT_FAILURE);
        }

		childPid = fork();
		if(-1 == childPid){
			perror("Error fork()");
			exit(EXIT_FAILURE);
		}

        while(0 != str_process){
            if(0 == childPid){  /*дочерний процесс*/
                close(fildes[1]);
                //dup2(fildes[0], 0);
               // errorExecv = execvp(tokens[0], tokens); /*начинаем выолняем команду после |*/
               /* if (errorExecv < 0){
                        perror("Error");
                        exit(EXIT_FAILURE);
                    }*/
                } else { /*родительский процесс*/
                    close(fildes[0]);
                    //dup2(fildes[1], 1);/*перенаправляем стандартный вывод в буфер канала*/
                    /*while((0 != strcmp(tokens[point], "|")) && (NULL != tokens[point+1])){ //считаем сколько аргументов до |
						point++;
                    }*/
                    //execvp(tokens,tokens[0]); /*выполняем программу*/

                    w = wait(&status); /*ждем пока выполнится дочерний процесс*/
                    if(-1 == w){
                        perror("error childPid");
                        exit(EXIT_FAILURE);
                    }
                }
        }
		free(str_process);
		free(tokens);	
	return 0;
}
