#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

#define AMOUNT_SHOP 5
#define AMOUNT_BUYER 3

//- прототипы фукнций потоков;
void* buy_product(void* thread_arg);
void* dop_product(void* thread_delivery);

struct buyer_info{
    int already_bought;
    int should_buy;
    int id;
};

//- пять глобальных магазинов, в которых будут совершены покупки;
int amount_products_shops[AMOUNT_SHOP];

// для мьютексов;
pthread_mutex_t mutex_shop[AMOUNT_SHOP] = {
                                    PTHREAD_MUTEX_INITIALIZER, 
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                    PTHREAD_MUTEX_INITIALIZER,
                                };

int main(void){

    int start_range_shop[2] = {905, 1050};
    int fin_range_buyer[2] = {4500, 5500};
    int delivery = 500;

    pthread_t thread[AMOUNT_BUYER + 1];

    int i = 0;

    struct buyer_info **buyer_info = malloc(AMOUNT_BUYER * sizeof(struct buyer_info*));

    srand(time(NULL));
    //- заполняем продуктами магзин;
    for(i = 0; i < AMOUNT_SHOP; i++){
        //- присваиваем рандомное значение продуктов для одного из магазино;
        amount_products_shops[i] = (random() % (start_range_shop[0] - start_range_shop[1] + 1)) + start_range_shop[0];
    }
    //- понимаем чего хотят покупатели;
    for(i = 0; i < AMOUNT_BUYER; i++){
        buyer_info[i] = malloc(sizeof(struct buyer_info));
        buyer_info[i]->already_bought = 0;
        //- присваиваем id покупателю
        buyer_info[i]->id = i + 1;
        //- присваиваем рандомное значение продуктов, к. нужно будет купить
        buyer_info[i]->should_buy = (random() % (fin_range_buyer[0] - fin_range_buyer[1] + 1)) + fin_range_buyer[0];

        pthread_create(&thread[i], NULL, buy_product, (void *)buyer_info[i]);
    }

    pthread_create(&thread[AMOUNT_BUYER], NULL, dop_product, (void *)&delivery);

    for(int i = 0; i < AMOUNT_BUYER; i++){
        pthread_join(thread[i], NULL);
    }
    pthread_cancel(thread[AMOUNT_BUYER]);

    printf("\n\n\n***\nПодведем итоги\n***\n\n\n");
    for(i = 0; i < AMOUNT_SHOP; i++){
        printf("В магазине %d осталось %d продуктов\n", i+1, amount_products_shops[i]);
    }
    printf("\n");
    for(i = 0; i < AMOUNT_BUYER; i++){
        printf("Покупатель %d хотел купить %d. После покупок у него %d продуктов\n", buyer_info[i]->id, buyer_info[i]->should_buy, buyer_info[i]->already_bought);
    }

    exit(EXIT_SUCCESS);
}

//- фукнция для потока покупателя;
void* buy_product(void* thread_buy){

    //- для проверки работы мьютекса;
    int result;
    //- счетчик
    int i = 0; 
    //- информация о покупателе
    struct buyer_info *buyer_info = (struct buyer_info *)thread_buy;
    //- сколько еще осталось купить;
    int eshe_buy = buyer_info->should_buy;

    while(buyer_info->should_buy >= buyer_info->already_bought){
        for(i = 0; i < AMOUNT_SHOP; i++){
            result = pthread_mutex_trylock(&mutex_shop[i]);
            if(0 == result){
                //- мьютекс успешно заблокирован;
                //printf("\n***\n");
                //printf("Покупатель %d зашел в магазин %d\n", buyer_info->id, i+1);
                //- проверяем есть ли в магазине продукты;
                if(0 == amount_products_shops[i]){
                    //printf("В магазине %d ничего нет :( Придется уходить покупателю %d\n", i+1, buyer_info->id);
                } else {
                    printf("\n***\n");
                    printf("Покупатель %d зашел в магазин %d\n", buyer_info->id, i+1);
                    printf("В магазине %d %d продуктов\n", i+1, amount_products_shops[i]);
                    printf("Покупатель %d все забирает\n", buyer_info->id);
                    buyer_info->already_bought += amount_products_shops[i];
                    eshe_buy -= amount_products_shops[i];
                    amount_products_shops[i] = 0;
                    printf("Покупателю %d осталось еще купить %d продуктов. Теперь он пошел спать\n", buyer_info->id, eshe_buy);
                    printf("\n***\n");
                    sleep(2);
                }
                //printf("\n***\n");
                pthread_mutex_unlock(&mutex_shop[i]);
                //- после покупки засыпаем на две секунды
            } else {
                //- уведомляем, что покупатель пошел в другой магазин;
                // printf("\n***\n");
                // printf("Покупатель %d пришел в магазин %d, но он не принимает гостей\nПридется идти в другой\n", buyer_info->id, i+1);
                // printf("\n***\n");
            }
        }
    }
}

//- фукнция для загрузки продуктов в магазины;
void* dop_product(void* thread_delivery){

    //- для проверки работы мьютекса;
    int result;
    //- счетчик
    int i = 0; 

    while(1/*тут условия пока у поккупателя нет достаточно покупок*/){
        for(i = 0; i < AMOUNT_SHOP; i++){
            result = pthread_mutex_trylock(&mutex_shop[i]);
            if(0 == result){
                //- мьютекс успешно заблокирован;
                //printf("\n***\n");
                //printf("Погрузчик подходит к магазину %d проверить еслть ли продукты\n", i+1);
                if(0 == amount_products_shops[i]){
                    printf("\n***\n");
                    printf("Погрузчик подходит к магазину %d проверить есть ли продукты\n", i+1);
                    printf("В магазине %d ничего нет :) Вперед за работу\n", i+1);
                    amount_products_shops[i] += 500;
                    printf("Все довольный работяга идет спать!");
                    printf("\n***\n");
                    sleep(1);   
                } else {
                    // printf("В магазине %d %d продуктов\n", i+1, amount_products_shops[i]);
                    // printf("Безработный погрузчик уходит");
                }
                //printf("\n***\n");
                pthread_mutex_unlock(&mutex_shop[i]);
                //- после загрузки засыпаем на одну секунду
                sleep(1);
            } else {
                //- уведомляем, что погрузчик пошел в другой магазин;
                // printf("\n***\n");
                // printf("Погрузчик пришел в магазин %d, а он занят. Ладно, не будем мешать\n", i+1);
                // printf("\n***\n");
            }
        }
    }
}