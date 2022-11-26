#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>

#define AMOUNT_THREADS 100/*максимальное количество потоков*/
#define perror(mes) do{perror(mes);\
			exit(EXIT_FAILURE);} while(0)
#define perror_arg(arg, mes) do{errno = arg; perror(mes);\
                        exit(EXIT_FAILURE);} while(0)

pthread_mutex_t m1 = PTHREAD_MUTEX_INITIALIZER;

struct infoThreads{
	pthread_t tid;
	int num;
};

struct int_data{
	int a;
};

/*прототипы функций*/
void *add(void *arg);

/*феализация функций*/
void *add(void *arg){
	struct int_data *incr = arg;
	pthread_mutex_lock(&m1);
		while(1000 > (incr->a)){
			printf("--> %ld\n", pthread_self());
			int tmp = incr -> a;
			tmp++;
			incr->a = tmp;
		}
	pthread_mutex_unlock(&m1);
        return 0;
}

int main(void){

	int num_pth;
	void *retval;
	int s; /*идентификатор создания потока*/
	struct infoThreads *tinfo;
	struct int_data int_a;

	int_a.a = 0;

	tinfo = calloc(AMOUNT_THREADS, sizeof(struct infoThreads));

	for(num_pth = 0; num_pth < AMOUNT_THREADS; num_pth++){
		tinfo[num_pth].num = num_pth + 1;

		s = pthread_create(&tinfo[num_pth].tid, NULL, &add, &int_a);
		if(0 != s)
			perror_arg(s, "error_create");
	}

	
	for(num_pth = 0; num_pth < AMOUNT_THREADS; num_pth++){
		s = pthread_join(tinfo[num_pth].tid, &retval);
		if(0 != s)
			perror_arg(s, "error_join");
	}
	

	printf("a = %d\n", int_a.a);

	free(tinfo);

	exit(EXIT_SUCCESS);
} 
