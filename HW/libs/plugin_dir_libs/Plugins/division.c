#include <stdio.h>
#include <stdlib.h>

char functionName[30] = "division";

char functionDescriptionMenu[] = "Функция деления двух чисел";

char functionDescription[] = "(Пример: Введите a и b: 3 2 Результат: 1)";

int division (int firstNumber, int secondNamber)
{
    int result;

    if (secondNamber != 0)
    {
        result = firstNumber / secondNamber;
    }
    else
    {
        printf("\nError: division by 0\n\n");
        exit(-1);
    }

    return result;
}