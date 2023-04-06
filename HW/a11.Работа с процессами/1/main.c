#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main(){

    pid_t pid;

    //- для wait();
    int status; 

    pid = fork();

    if (0 == pid){
        printf("    -2- pid:%d, ppid:%d\n\n", getpid(), getppid());
        pid = fork();
        if(0 == pid){
            printf("        -4- pid:%d, ppid:%d\n\n", getpid(), getppid());
        } else if(0 < pid){
            pid = fork();

            if(0 == pid){
                printf("        -5- pid:%d, ppid:%d\n\n", getpid(), getppid());
            }else if(0 < pid){
                    // тут что-то делает второй родитель;
            }else if(0 > pid){
                perror("1");
                exit(EXIT_FAILURE);
            }
            wait(&status);
            if(WIFEXITED(status) < 0){
                perror("status1");
                exit(EXIT_FAILURE);
            }
        } else if(0 > pid){
            perror("3");
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if(WIFEXITED(status) < 0){
            perror("status1");
            exit(EXIT_FAILURE);
        }
    } else if(0 < pid){
        printf("-1- pid:%d, ppid:%d\n\n", getpid(), getppid());
        pid = fork();
        if(0 == pid){
            printf("    -3- pid:%d, ppid:%d\n\n", getpid(), getppid());

            pid = fork();
            if(0 == pid){
                printf("        -6- pid:%d, ppid:%d\n\n", getpid(), getppid());
            }else if(0 < pid){
                //- тут что-то делает третий родитель;
            }else if(0 < pid){
                perror("status1");
                exit(EXIT_FAILURE);
            }
            wait(&status);
            if(WIFEXITED(status) < 0){
                perror("status1");
                exit(EXIT_FAILURE);
            }

        }else if(0 < pid){
            //- тут должен что-то делать самый первый родитель;
        }else if(0 > pid){
            perror("2");
            exit(EXIT_FAILURE);
        }
        wait(&status);
        if(WIFEXITED(status) < 0){
            perror("status1");
            exit(EXIT_FAILURE);
        }

    } else if(0 > pid){
        perror("1");
        exit(EXIT_FAILURE);
    }
    wait(&status);
    if(WIFEXITED(status) < 0){
        perror("status1");
        exit(EXIT_FAILURE);
    }

    exit(EXIT_SUCCESS);
}