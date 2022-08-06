#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#define AMOUNT_SHOPS 5
#define AMOUNT_BUYER 10
#define STOCK_PRODUCT 10000000
#define TARGET 10000
#define START_PRODUCTS 1000

#define perror(mes) do{ perror(mes); exit(EXIT_FAILURE);} while(0);
#define perror_arg(arg, mes) do{errno = arg; perror(mes); exit(EXIT_FAILURE);} while(0)

pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex4 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex5 = PTHREAD_MUTEX_INITIALIZER;

//pthread_mutex_t mutexs[5] = {mutex1, mutex2, mutex3, mutex4, mutex5};

struct trade{
        int goods;
};

struct buyer{
	int needBuy;
	int current_product;
	struct trade **infoshop;
};

struct refiller{
	int store;
	struct trade **infoshop;
};


/*прототипы функций*/

void *buy_product(void *arg); /*функция для покупки*/
void *refill(void *arg); /*функция для пополнения продуктов*/

/*главная функция*/

int main(void){

	pthread_t tid_byer[AMOUNT_BUYER];
	pthread_t tid_refiller;
	int s; /*переменная куда будет возвращаться информация о потоках*/
	int nBuyer; /*порядковый номер покупателя*/
	int nShop; /*порядковый номер магазина*/
	void *res;

	struct buyer *buyer_main; /*создаем указатели на структуры или объекты 
					с которыми будем работать*/
	struct refiller *refiller_main;
	struct trade *shop_main; /*объект магазины*/

	

	buyer_main = calloc(AMOUNT_BUYER, sizeof(struct buyer)); /* выделяем область
									   под объекты */
	if(NULL == buyer_main)
		perror("error_calloc_buyermain");
	
	refiller_main = malloc(sizeof(struct refiller));
	if(NULL == refiller_main)
                perror("error_calloc_refillermain");
	
	shop_main = calloc(AMOUNT_SHOPS, sizeof(struct trade));
        if(NULL == shop_main)
                perror("error_calloc_shop");
	
	for(int nShop = 0; nShop < AMOUNT_SHOPS; nShop++){
		shop_main[nShop].goods = 1000;
	}
	
	refiller_main->store = STOCK_PRODUCT;
	refiller_main->infoshop = &shop_main;
	/*создадим потоки*/

	s = pthread_create(&tid_refiller, NULL, &refill, &refiller_main);
	if(0 != s)
		perror_arg(s, "error_create_refiller");
	for(int nBuyer = 0; nBuyer < AMOUNT_BUYER; nBuyer++){
		buyer_main[nBuyer].infoshop = &shop_main;
		buyer_main[nBuyer].needBuy = 10000;
		buyer_main[nBuyer].current_product = 0;	
		
		s = pthread_create(&tid_byer[nBuyer], NULL, &buy_product, &buyer_main);
		if(0 != s)
			perror_arg(s, "error_pthread_create_buyer");
	}

	s = pthread_join(tid_refiller, &res);
	if(0 != s)
		perror_arg(s, "join_reffiler");
	free(res);
	for(int nBuyer = 0; nBuyer < AMOUNT_BUYER; nBuyer++){	
		s = pthread_join(tid_refiller, &res);
		if(0 != s)
	                perror_arg(s, "join_buyer");
		free(res);
	}
	
	free(buyer_main);
	free(refiller_main);

	pthread_mutex_destroy(&mutex1);
	pthread_mutex_destroy(&mutex2);
	pthread_mutex_destroy(&mutex3);
	pthread_mutex_destroy(&mutex4);
	pthread_mutex_destroy(&mutex5);

	exit(EXIT_SUCCESS);
}

/*реализация функций*/

void *buy_product(void *arg){

	struct buyer *dai = arg; 
	
	struct trade **info = dai -> infoshop;
	
	int nShop;

	pthread_mutex_t mutexs[5] = {mutex1, mutex2, mutex3, mutex4, mutex5};	

	while((dai -> needBuy) < (dai -> current_product)){
		for(nShop = 0; nShop < AMOUNT_SHOPS; nShop++){
			if(0 < ((*info)[nShop].goods)){
				
				pthread_mutex_trylock(&mutexs[nShop]);
        	               		dai->current_product += (*info)[nShop].goods;
					(*info)[nShop].goods = 0;
	        	        pthread_mutex_unlock(&mutexs[nShop]);
				sleep(2);
			}
		}

	}
	
	return 0;
}

void *refill(void *arg){

	struct refiller *get = arg;
	struct trade **info = get -> infoshop;
	pthread_mutex_t mutexs[5] = {mutex1, mutex2, mutex3, mutex4, mutex5};
	int nShop;
	
	while(1)
	{ 
		for(nShop = 0; nShop < AMOUNT_SHOPS; nShop++)
		{
			if(0 == ((*info)[nShop].goods)){
				pthread_mutex_trylock(&mutexs[nShop]);
       		                 	(*info)[nShop].goods += 1000;
					printf("В магазин %d было добавлено 1000\n", nShop);
       	                	pthread_mutex_unlock(&mutexs[nShop]);
			}
		}
	}
	return 0;
}

