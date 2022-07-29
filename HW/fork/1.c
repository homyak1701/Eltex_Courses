#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>

int main(void)
{
	int status_priority; /*приоритет родительского процесса*/
	int status;
	pid_t current_pid; /*id родительского процесса*/
	pid_t child_pid; /*id дочернего процесса*/

	current_pid = getpid(); /*получение id текущего процесса*/

	status_priority = getpriority(PRIO_PROCESS, current_pid); /*получение приоритета текущего процесса*/


	printf("\n1) id process = %d\n", current_pid);
	printf("\n   id parent process = %d\n", getppid());
	printf("\n   for the current process prioryty = %d\n", status_priority);	
	

	child_pid = fork(); /*родителю возвращется pid потомка, а потомку 0*/
	if (-1 == child_pid){
		perror("no_fork");
		exit(EXIT_FAILURE);
	}

	if (0 == child_pid){
			
		printf("\n id child id = %d\n", getpid());
		printf("\n parent id for child = %d\n", getppid());
		status_priority = getpriority(PRIO_PROCESS, getpid());
		printf("\n prioryty for the child process = %d\n", getpriority(PRIO_PROCESS,status_priority));

	} else {	/*parent process*/
		current_pid = wait(&status);
		
		if(-1 == child_pid){
			perror("bad_wait");
			exit(EXIT_FAILURE);
		}
		
		printf("\nid process = %d\n", getpid());
	        printf("\nid parent process = %d\n", getppid());
	        printf("\nfor the current process prioryty = %d\n", getpriority(PRIO_PROCESS, getpid()));
	}

	return 0;
}
