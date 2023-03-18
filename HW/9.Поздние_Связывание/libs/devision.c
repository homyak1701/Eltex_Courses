#include <stdio.h>

int AMOUNT = 1;

char *name_funcs[] = {"devision"};

char *description[] = {"Деление"};

int devision(){

    printf("Введите два числа, которые хотите разделить");

    int a = 0;
    int b = 0;
    
    int devision = 0;

    printf("\n");
    printf("---> ");
    scanf("%d", &a);
    printf("\n");

    printf("---> ");
    scanf("%d", &b);
    printf("\n");

    devision = a / b;

    printf("Результат ---> %d\n", devision);

    return 0;
}