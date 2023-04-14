#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>


int main(){

    pid_t pid_1, pid_2;
    //- для wait();
    int status;
    //- для сохранения дескриптора вывода;
    int std_out, std_in;
    //- для ввода пользователя;
    char input[100];
    //- для отделения самой команды;
    char command_parent[100];
    char command_child[100];
    //- для одного ключа команды;
    char key_parent[100];
    char key_child[100];
    //- путь к папке
    char path[100] = "/bin/";
    //- массив, которые будет хранить полный путь к команде;
    char full_path[100];
    //- для идентификации пробелов;
    int i, i2;
    //- для каналов;
    int fd[2];


    while(1){
        printf("\n\n(Введите стандартуную команду bash из папки /bin/)#");

        memset(input, 0, sizeof(input));
        memset(command_parent, 0, sizeof(command_parent));
        memset(command_child, 0, sizeof(command_child));
        memset(key_parent, 0, sizeof(key_parent));
        memset(key_child, 0, sizeof(key_child));

        i = 0;
        i2 = 0;

        fgets(input, sizeof(input), stdin);
        if((0 == strcmp(input, "exit\n"))){
            break;
        }

        //- зануляем полный путь предыдущей команды;
        memset(full_path, 0, sizeof(full_path));
        strcat(full_path, path);

        //- выделяем команду родителя
        while((input[i] != ' ') && (input[i] != '\n')){
            i++;
        }
        strncpy(command_parent, input, i);
        i++;
        //- выделяем единственный ключ родителя
        if(input[i] == '|'){
            key_parent[i2] = '\000';
        } else {
            while((input[i] != '\n') && (input[i] != 0) && (input[i] != '|') && (input[i] != ' ')){
                key_parent[i2] = input[i];
                i++;
                i2++;
            } 
            //- так как i сейчас находится на пробеле, переместим куссор на 1 вперед;
            i++;
        }

        //- на всякий случай сохраняем значение дескриптора 
        //стандартного потока вывода/вывода;
        std_out = dup(STDOUT_FILENO);
        std_in = dup(STDIN_FILENO);

        pid_1=fork();

        if(0 == pid_1){
            //- если есть конвейер
            if(('|' == input[i]) && (' ' == input[i+1])){

                //- если есть конвейер, то создаем канал
                if(-1 == pipe(fd)){
                    perror("pipe_create");
                    exit(EXIT_FAILURE);
                }

                pid_2=fork();

                //- дочерний процесс, который принимает данные из канала;
                if(0 == pid_2){

                    close(fd[1]);

                    if(dup2(fd[0], STDIN_FILENO) < 0){
                            perror("замена STDIN_FILENO на fd[0]");
                            exit(EXIT_FAILURE);
                    }
                    //- доходим до начала нашей второй команды
                    i++;
                    i++;
                    i2 = 0;
                    //- выделяем команду родителя
                    while((input[i] != ' ') && (input[i] != '\n')){
                        command_child[i2] = input[i];
                        i2++;
                        i++;
                    }
                    i2 = 0;
                    i++;
                    //- выделяем единственный ключ родителя
                    while((input[i] != '\n') && (input[i] != 0)){
                        key_child[i2] = input[i];
                        i++;
                        i2++;
                    } 
                    if('\000' != key_child[0]){
                        printf("Дочерний процесс потомка: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                        //- запускаем команду bash
                        execl(strcat(path, command_child), command_child, key_child, NULL);
                    } else {
                        printf("Дочерний процесс потомка: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                        //- запускаем команду bash
                        execl(strcat(path, command_child), command_child, NULL);
                    }       
                } else if(0 < pid_2){

                    printf("Родительский процесс (2): pid: %d, ppid: %d\n", getpid(), getppid());

                    //- у родительского процесса потомка закрываем дескриптор на запись;
                    close(fd[0]);

                    if(dup2(fd[1], STDOUT_FILENO) < 0){
                        perror("замена STDOUT_FILENO на fd[1]");
                    }

                    if('\000' != key_parent[0]){
                        printf("Дочерний процесс родителя: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                        //- запускаем команду bash
                        strcat(full_path, command_parent);
                        execl(full_path, command_parent, key_parent, NULL);
                    } else {
                        printf("Дочерний процесс родителя: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                        //- запускаем команду bash
                        strcat(full_path, command_parent);
                        execl(full_path, command_parent, NULL);
                    }
                } else if(0 > pid_2){
                    perror("fork");
                    exit(EXIT_FAILURE);
                }
                wait(&status);
                if (!WIFEXITED(status)) {
                    printf("\n\nНенормальное завершение дочернего процесса потомка\n");
                }
            //- если конвейера нет, то просто выполняет код
            } else {
                if('\000' != key_parent[0]){
                    printf("Дочерний процесс родителя: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                    //- запускаем команду bash
                    strcat(full_path, command_parent);
                    execl(full_path, command_parent, key_parent, NULL);
                } else {
                    printf("Дочерний процесс родителя: pid: %d, ppid: %d\n\n\n", getpid(), getppid());
                    //- запускаем команду bash
                    strcat(full_path, command_parent);
                    execl(full_path, command_parent, NULL);
                }
            }
        } else if(0 < pid_1){
            printf("Родительский процесс (1): pid: %d, ppid: %d\n", getpid(), getppid());
        } else if(0 > pid_1){
            perror("fork");
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if (!WIFEXITED(status)) {
            printf("\n\nНенормальное завершение дочернего процесса родителя\n");
        }

        if(dup2(std_out, STDOUT_FILENO) < 0){
            perror("замена fd[1] на STDIN_FILENO");
            exit(EXIT_FAILURE);
        }
        if(dup2(std_in, STDIN_FILENO) < 0){
            perror("замена fd[0] на STDIN_FILENO");
            exit(EXIT_FAILURE);
        }
    }
    exit(EXIT_SUCCESS);
    sleep(1);
}