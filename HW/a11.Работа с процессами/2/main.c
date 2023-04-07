#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

int main(){

    pid_t pid;
    //- для wait();
    int status;
    //- для ввода пользователя;
    char input[100];
    //- для отделения самой команды;
    char command[100];
    //- для одного ключа;
    char key[100];
    //- путь к папке
    char path[100] = "/bin/";
    //- массив, которые будет хранить полный путь к команде;
    char full_path[100];
    //- для идентификации пробелов;
    int i, i2;

    memset(input, 0, sizeof(input));
    memset(command, 0, sizeof(command));
    memset(key, 0, sizeof(key));
    memset(full_path, 0, sizeof(full_path));


    while(1){
        printf("(Введите стандартуную команду bash из папки /bin/)#");
        fgets(input, sizeof(input), stdin);
        if((0 == strcmp(input, "exit\n"))){
            break;
        }
        i = 0;
        i2 = 0;
        //- зануляем предыдущую работу;
        memset(full_path, 0, sizeof(full_path));
        strcat(full_path, path);
        //- выделяем команду
        while((input[i] != ' ') && (input[i] != '\n')){
            i++;
        }
        strncpy(command, input, i);
        i++;
        //- выделяем единственный ключ
        while((input[i] != '\n') && (input[i] != 0)){
            key[i2] = input[i];
            i++;
            i2++;
        } 
        pid=fork();
        if(0 == pid){
            if('\000' != key[0]){
                printf("Дочерний процесс: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                //- запускаем команду bash
                execl(strcat(path, command), command, key, NULL);
            } else {
                printf("Дочерний процесс: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                //- запускаем команду bash
                execl(strcat(path, command), command, NULL);
            }
        } else if(0 < pid){
            printf("Родительский процесс: pid: %d, ppid: %d\n", getpid(), getppid());
        } else if(0 > pid){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if (WIFEXITED(status)) {
            printf("\n\nДочерний процесс завершился со статусом %d\n", WEXITSTATUS(status));
        } else {
            printf("\n\nНенормальное завершение дочернего процесса\n");
        }
    }
    exit(EXIT_SUCCESS);
    sleep(1);
}