#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define AMOUNT_PTHREADS 5
#define perror_arg(arg, mes) do{ errno = arg;  perror(mes);  \
		exit(EXIT_FAILURE);	} while(0)
#define perror(mes) do {  perror(mes);\
		exit(EXIT_FAILURE);	} while(0)

struct info_threads {
	pthread_t thread_id;
	int num_threads;
};

/*прототип функции для потока*/


/*реализация функций*/

void *getId(void*arg){
	
	struct info_threads *tinfo = arg;

	printf("\nid_threads = %ld, num_threads = %d\n", tinfo->thread_id, tinfo->num_threads); 
}

int main(void){

	int n_pth;
	int s;
	struct info_threads *tinfo;
	void *retval;


	tinfo = calloc(AMOUNT_PTHREADS, sizeof(struct info_threads));
	if (tinfo == NULL)
		perror("error_calloc");



	for(n_pth = 0; n_pth < AMOUNT_PTHREADS; n_pth++){
		tinfo[n_pth].num_threads = n_pth + 1;
		
		s = pthread_create(&tinfo[n_pth].thread_id, NULL, &getId, &tinfo[n_pth]);	
		if (s != 0)
			perror_arg(s, "error_pthread_create");
	}

	for(n_pth = 0; n_pth < AMOUNT_PTHREADS; n_pth++){
		s = pthread_join(tinfo[n_pth].thread_id, &retval);
		if(0 != s)
			perror_arg(s, "error_pthread_join");
	}


	free(tinfo);

	exit(EXIT_SUCCESS);
}
