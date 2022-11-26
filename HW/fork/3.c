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
char* readline();
char** split(char* line);

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

char** split(char* line){

	size_t buf_size = DEFAULT_BUFF_SIZE; /*изначальный размер буфера для токенов*/
	size_t point = 0;/*счетчик токинов*/

	char* token = NULL; /*будет содержать все ссылки на токены строки*/

	char** tokens = (char **)malloc(sizeof(char *)*DEFAULT_BUFF_SIZE);
	if (tokens == NULL){
		printf("[ERROR] Couldn't allocated buffer for spliting");
		return NULL;
	}


	token = strtok(line, TOKENS_DELIMITERS);
	
	while(0 != token){
		tokens[point++] = token;

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
		token = strtok(NULL, TOKENS_DELIMITERS);
	}

	tokens[point] = NULL; /*в конец токенов добавляем NULL, так как функция exec() 
	ожидает наличие данного завершающего токена*/

	return tokens;
}

int main (int argc, char **argV)
{
	system("clear");
	while( 1 ){

		pid_t childPid = 0;
		pid_t w;
		int nArgc = 0;
		int errorExecv;
		int status; /*для wait*/
		char **tokens = NULL;
		char *string_comands = NULL;
		
		printf("\nEnter your comanl --> ");
		string_comands = readline();
		
		tokens = split(string_comands);
		
		childPid = fork();
		if(-1 == childPid){
			perror("Error fork()");
			exit(EXIT_FAILURE);
		}
		/*проверяем есть ли второй аргумент и если он есть выполняем процесс с аргументом*/
		if(NULL != tokens[1]){
			if(0 == childPid){
				errorExecv = execlp(tokens[0],tokens[0], tokens[1], NULL);
			        if (errorExecv < 0){
	  	                	perror("Error");
		  		        exit(EXIT_FAILURE);
				}
			} else {
				w = wait(&status);
				if(-1 == w){
					perror("error childPid");
					exit(EXIT_FAILURE);
				}
			}
		} else{ /*если нет аргумента просто выполняем процесс*/
			if(0 == childPid){
                                errorExecv = execlp(tokens[0],tokens[0], NULL);
                                if (errorExecv < 0){
	                                perror("Error");
        	                        exit(EXIT_FAILURE);
				}
			} else {
				w = wait(&status);
				if(-1 == w){
					perror("error childPid");
					exit(EXIT_FAILURE);
				}
			}
		}



		free(tokens);	
	}

	return 0;
}
