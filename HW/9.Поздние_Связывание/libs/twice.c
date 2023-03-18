#include <stdio.h>

int AMOUNT = 2;

char *name_funcs[] = {"twice1","twice2"};

char *description[] = {"Умножение на два", "Умножение на четыре"};

int twice1(){

    printf("Введите число, которое нужно умножить на 2");

    int a = 0;
    int twice1 = 0;

    printf("\n");
    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    twice1 = a * 2;

    printf("Результат ---> %d\n", twice1);

    return 0;
}

int twice2(){

    printf("Введите число, которое нужно умножить на 4");

    int a = 0;
    int twice2 = 0;

    printf("\n");
    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    twice2 = a * 4;

    printf("Результат ---> %d\n", twice2);

    return 0;
}