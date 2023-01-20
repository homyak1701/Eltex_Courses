#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <malloc.h>

#define N 10
//макрос по нахождению ошибки при выделении памяти
#define error_memory(a) do{if(NULL == a){  printf("line:%d\n", __LINE__); \
                                        perror("memory"); exit(EXIT_FAILURE);}} while(0)


enum chose_users{
                    NEW_USER = 1,
                    SEARCH_USER,
                    LIST_USERS,
                    DEL_USER,
                    EXIT
                };

struct subscribers{
    char first_name[10];
    char second_name[10];
    char number[10];
};

int main(){

    //- указатель на всю область справочника;
    struct subscribers *point_list = (struct subscribers *)malloc(sizeof(struct subscribers) * 100);
        error_memory(point_list);
    //- указатель, который будет содержать ссылку только на одного
    //пользователя
    struct subscribers *point_user = NULL;

    //- переменная куда бедет записыватся выбор пользователя;
    enum chose_users scanf_for_switch;
    //- используется для поиска, а точнее сравнивания
    //что ввел пользователь и исходными данными;
    char search_user[50];
    //- то что возвращает функции при сравнивании строк;
    int cmpr_string;
    //переменная счетчик
    int i = 0;
    //- количетсво зарегистрированных записей
    int amount = 0;

    memset(search_user, 0, 50);

    printf("*****\n");
    printf("Hello!\n");
    printf("*****\n");
    
    while(1){

    printf("*****\n");
    printf("Press\n 1-new user\n 2-search user\n 3-list users\n 4-del user\n 5-exit\n");
    printf("*****\n-----> ");

    //- стандартный поток ввода для выбора пользователя
    scanf("%d", (int *)&scanf_for_switch);

        switch(scanf_for_switch){
            case NEW_USER:
                //- сдвигаем указатель пользователя на нужную
                //область структуры;
                point_user = point_list;
                point_user = point_user + amount;
                memset(point_user, 0, sizeof(struct subscribers));
                printf("Впишите данные пользователя\n");
                printf("Имя: "); 
                scanf("%s", point_user->first_name);
                printf("\nФамилия: "); 
                scanf("%s", point_user->second_name);
                printf("\nНомер телефона: "); 
                scanf("%s", point_user->number);
                printf("\n");
                //- увеличиваем справочник на одного пользователя
                //чтобы в дальнейшем было место, куда записывать
                point_list = (struct subscribers *)realloc(point_list, sizeof(struct subscribers));
                        error_memory(point_list);
                amount++;
                break;                     
            case SEARCH_USER:
                printf("\nВведите имя пользователя, которого хотите найти: ");
                scanf("%s", search_user);
                point_user = point_list;
                for(i=0; i < amount; i++){
                    cmpr_string = strcmp(search_user,point_user->first_name);
                    if(0 == cmpr_string){
                        printf("------\n");
                        printf("Данные пользователя %d\n", i+1);
                        printf("Имя: %s", point_user->first_name);
                        printf("\nФамилия: %s", point_user->second_name);
                        printf("\nНомер телефона: %s", point_user->number);
                        printf("\n------\n\n\n\n\n\n");
                        break;
                    }
                    point_user++;
                }
                if(amount-1 == i){
                    printf("Такого пользователя нет\n");
                    break;
                }
                break;
            case LIST_USERS:
                point_user = point_list;
                for (i = 0; i < amount; i++){       
                        printf("------\n");
                        printf("Данные пользователя %d\n", i+1);
                        printf("Имя: %s", point_user->first_name);
                        printf("\nФамилия: %s", point_user->second_name);
                        printf("\nНомер телефона: %s", point_user->number);
                        printf("\n------\n\n\n");
                        point_user++; 
                }
                break;
            case DEL_USER:
                printf("\nВведите имя пользователя, которого хотите удалить:");
                scanf("%s", search_user);
                // point_user = point_list;
                // for(i=0; i < amount; i++){
                //     cmpr_string = strcmp(search_user,point_user->first_name);
                //     if(0 == cmpr_string){
                //         for(i; i<amount; i++){
                //             point_user->first_name = (point_user+1)->first_name;
                //         }
                //         amount--;
                //         printf("\n!!!\nПользователь удален\n!!!\n");
                //         break;
                //     }
                //     if(N-1 == i){
                //         printf("Такого пользователя нет\nНичего не удалилось\n");
                //         break;
                //     }
                // }
                break;
            case EXIT:
                free(point_list);
                exit(0);
            default:
                printf("Вы ввели неправильное значение, попробуйте снова\n");
                break;
        }

    }
    return 0;
}