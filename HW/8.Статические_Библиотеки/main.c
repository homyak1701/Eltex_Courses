#include <stdio.h>
#include <stdlib.h>
#include "calculator.h"

enum chose_do_user{
                    PLUS = 1,
                    MINUS,
                    MULTIPLY,
                    DIVISION,
                    EXIT,
};

int main()
{
    //- числа, с которыми будем работать;
    int a = 0;
    int b = 0;
    //- переменная для результата
    float result = 0;
    //- выбор пользователя;
    int chose_case;

    printf("****\nДобро пожаловать!\n****\nВыберите действие\n\n");

    while(1)
    {
        printf("1.Сложить\n2.Вычесть\n3.Умножить\n4.Разделить\n5.Выход\n\n---> ");
        scanf("%d", &chose_case);
        printf("\n");
        switch(chose_case)
        {
            case PLUS:
                printf("Введите значение: а = ");
                scanf("%d", &a);
                printf("\nВведите значение: b = ");
                scanf("%d", &b);
                printf("\n");
                result = plus(a, b);
                printf("Результат: a + b = %d\n\n", (int)result);
                break;
            case MINUS:
                printf("Введите значение: а = ");
                scanf("%d", &a);
                printf("\nВведите значение: b = ");
                scanf("%d", &b);
                printf("\n");
                result = minus(a, b);
                printf("Результат: a + b = %d\n\n", (int)result);
                break;
            case MULTIPLY:
                printf("Введите значение: а = ");
                scanf("%d", &a);
                printf("\nВведите значение: b = ");
                scanf("%d", &b);
                printf("\n");
                result = multiply(a, b);
                printf("Результат: a + b = %d\n\n", (int)result);
                break;
            case DIVISION:
                printf("Введите значение: а = ");
                scanf("%d", &a);
                printf("\nВведите значение: b = ");
                scanf("%d", &b);
                printf("\n");
                result = division(a, b);
                printf("Результат: a + b = %f\n\n", result);
                break;
            case EXIT:
                printf("Всего хо-ро-ше-го\n");
                exit(0);
                break;
            default:
                printf("\nВы ввели не верное число\n\n");
        }
    }

    return 0;
}