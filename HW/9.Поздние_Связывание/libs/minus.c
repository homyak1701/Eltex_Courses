#include <stdio.h>

int AMOUNT = 1;

char *name_funcs[] = {"minus"};

char *description[] = {"Вычитание"};

int minus(){
    
    printf("Введите два число, которые хотите вычесть\n");

    int a = 0;
    int b = 0;
    
    int minus = 0;

    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    printf("---> ");
    scanf("%d", &b);
    printf("\n");
    
    minus = a - b;
    printf("Результат ---> %d\n", minus);

    return minus;
}