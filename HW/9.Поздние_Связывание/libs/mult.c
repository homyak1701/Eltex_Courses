#include <stdio.h>

int AMOUNT = 1;

char *name_funcs[] = {"mult"};

char *description[] = {"Умножение"};

int mult(){
    
    printf("Введите два число, которые хотите умножить\n");

    int a = 0;
    int b = 0;
    
    int mult = 0;

    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    printf("---> ");
    scanf("%d", &b);
    printf("\n");

    mult = a * b;

    printf("Результат ---> %d\n", mult);

    return mult;
}