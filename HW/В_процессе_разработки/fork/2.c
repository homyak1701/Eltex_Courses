/*создание дерева процессов*/


#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/resource.h>


int main ()
{
	pid_t child2, child3, child2_1, child3_1, child3_2; /*индификаторы дочерних процессов, где n_z*/
							    /*n - потомок от глав. процесса;*/
							    /*z - потомок от первого потомка*/
	pid_t w;
	int status; /*сюда будет возращаться статус завершения потомка*/
	

	printf("\nmain -- id process = %d\n", getpid());
        printf("id parent process = %d\n", getppid());
        printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));

	child2 = fork();
	if (-1 == child2){
		perror("error_fork_child2");
		exit(EXIT_FAILURE);
	}	

	if (0 == child2){
		printf("\n2 -- id process = %d\n", getpid());
	        printf("id parent process = %d\n", getppid());
	        printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));

		child2_1 = fork();
		if(-1 == child2_1){
			perror("fork_child2_1");
			exit(EXIT_FAILURE);	
		} 
		
		if (0 == child2_1)
		{
			printf("\n2_1 -- id process = %d\n", getpid());
                	printf("id parent process = %d\n", getppid());
	                printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));
		} else{
			w = wait(&status);
			if(-1 == w){
				perror("error_process_child2_1");
				exit(EXIT_FAILURE);
			}
		}
	} else {
		w = wait(&status);
		if(-1 == w){
			perror("wait_1");
			exit(EXIT_FAILURE);
		}
		
		child3 = fork();
		if (-1 == child3){
			perror("fork_child3");
			exit(EXIT_FAILURE);	
		}

		if(0 == child3){
			printf("\n3 -- id process = %d\n", getpid());
                        printf("id parent process = %d\n", getppid());
                        printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));

			child3_1 = fork();
			if (-1 == child3_1){
                	        perror("fork_child3_1");
        	                exit(EXIT_FAILURE);
	                }

			if (0 == child3_1){
				printf("\n3_1 -- id process = %d\n", getpid());
	                        printf("id parent process = %d\n", getppid());
        	                printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));
			} else {
				w = wait(&status);
				if (-1 == w){
					perror("process_child3_1");
					exit(EXIT_FAILURE);
				}

				child3_2 = fork();
				if(-1 == child3_2){
					perror("fork_child3_2");
					exit(EXIT_FAILURE);
				}

				if (0 == child3_2){
					printf("\n3_2 -- id process = %d\n", getpid());
        	                        printf("id parent process = %d\n", getppid());
	                                printf("for the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));
				} else {
					w = wait(&status);
					if(-1 == w){
						perror("process_child3_2");
						exit(EXIT_FAILURE);
					}
				}
			}
		} else {
			w = wait(&status);
			if(-1 == w){
				perror("process_child3");
				exit(EXIT_FAILURE);
			}
		}
	}
	return 0;
}
