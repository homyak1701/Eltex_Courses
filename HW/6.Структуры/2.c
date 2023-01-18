#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10

enum chose_users {
                    NEW_USER = 1,
                    SEARCH_USER,
                    LIST_USERS,
                    DEL_USERS,
                    EXIT
                };

struct subscribers{
    char first_name[50];
    char second_name[50];
    char number[10];
    int using;
};

int yyyyy;

int main(){

    struct subscribers users[N];
    //- переменная куда бедет записыватся выбор пользователя;
    enum chose_users scanf_for_switch;
    //- используется для поиска, а точнее сравнивания
    //что ввел пользователь и исходными данными;
    char search_user[50];
    //переменная счетчик
    int i = 0;

    memset(&users, 0, sizeof(struct subscribers)*N);
    memset(search_user, 0, 50);

    printf("*****\n");
    printf("Hello!\n");
    printf("*****\n");
    
    while(1){

    printf("*****\n");
    printf("Press\n 1-new user\n 2-search user\n 3-list users\n 4-del user\n 5-exit\n");
    printf("*****\n-----> ");

    scanf("%d", (int *)&scanf_for_switch);

    switch(scanf_for_switch){
        case NEW_USER:
            for (i = 0; i < N; i++){
                //- если пользователь не используется то
                //записываем новые данные;
                if(0 == users[i].using){
                    users[i].using = 1;-
                    printf("Впишите данные пользователя\n");
                    printf("Имя: "); 
                    scanf("%s", users[i].first_name);
                    printf("\nФамилия: "); 
                    scanf("%s", users[i].second_name);
                    printf("\nНомер телефона: "); 
                    scanf("%s", users[i].number);
                    printf("\n");
                    break;printf("*****\n");
                }

                if((i == N-1) && (1 == users[i].using)){
                    printf("Место под данные закончились\n");
                }
            }
            break;
        case SEARCH_USER:
            printf("\nВведите имя пользователя, которого хотите найти: ");
            scanf("%s", search_user);

            for(i=0; i < N; i++){

                yyyyy = strcmp(search_user,users[i].first_name);
                if(0 == yyyyy){
                    printf("------\n");
                    printf("Данные пользователя %d\n", i+1);
                    printf("Имя: %s", users[i].first_name);
                    printf("\nФамилия: %s", users[i].second_name);
                    printf("\nНомер телефона: %s", users[i].number);
                    printf("\n------\n\n\n\n\n\n");
                    break;
                }

                if(N-1 == i){
                    printf("Такого пользователя нет\n");
                }

            }
            break;
        case LIST_USERS:
            for (i = 0; i < N; i++){
                if(1 == users[i].using){
                    printf("------\n");
                    printf("Данные пользователя %d\n", i+1);
                    printf("Имя: %s", users[i].first_name);
                    printf("\nФамилия: %s", users[i].second_name);
                    printf("\nНомер телефона: %s", users[i].number);
                    printf("\n------\n\n\n\n\n\n");
                }
            }
            break;
        case DEL_USERS:
            printf("\nВведите имя пользователя, которого хотите удалить:");
            scanf("%s", search_user);

            for(i=0; i < N; i++){

                yyyyy = strcmp(search_user,users[i].first_name);
                if(0 == yyyyy){
                    users[i].using = 0;
                    printf("\n!!!\nПользователь удален\n!!!\n");
                    break;
                }
                if(N-1 == i){
                    printf("Такого пользователя нет\nНичего не удалилось\n");
                }

            }
            break;
        case EXIT:
            exit(0);
        default:
            printf("Вы ввели неправильное значение, попробуйте снова\n");
            break;
    }

    }

    return 0;
}