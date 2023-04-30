#include <stdio.h>
#include <pthread.h>
#include <string.h>

//- количетсво потоков;
#define NUM_THREAD 10

//- глобальная переменная, коотрую будем изменять;
long a = 0;

void *thread_start(void *arg){
    int i;
    int tmp;

    for(i = 0; i < 1000000; i++){
        tmp = a; 
        tmp++;
        a = tmp;
    }

    printf("--> index = %d\n", *((int *)arg));

    //pthread_exit(NULL);
    return NULL;
}

int main(void){

    pthread_t thread[NUM_THREAD];
    int i;
    int index[NUM_THREAD];
    
    for(i = 0; i < NUM_THREAD; i++){
        index[i] = 0;
    }

    int index1;
    int *s;

    for(i=1; i<=NUM_THREAD; i++){
        index[i-1] = i;
        pthread_create(&thread[i-1], NULL, thread_start, &index[i-1]);
    }

    for(index1=0; index1 < NUM_THREAD; index1++){
        pthread_join(thread[index1], (void **) &s);
    }

    printf("a = %ld\n", a);
}