//вывод - так как 

#include <stdio.h>
 
int main()
{

    int input = 4;

    switch ( input ) {
        case 1000:            /* обратите внимание на двоеточие, точки сзапятой тут нет */
            printf("«His Death Was Just The Beginning»\n");
            break;
        case 100000:
            printf("«Scary Just Got Sexy»\n");
            break;
        case 3000:
            printf("«Fight the dead. Fear the living»\n");
            break;
        case 4:
            printf( "Сегодня смотреть ничего не б:(\n" );
            break;
        case 5:
            printf( "Сегодня смотреть нибудем :(\n" );
            break;
        case 6:
            printf( "Сегодня смотрчего не будем :(\n" );
            break;
        case 7:
            printf( "Сегодня смотeeть ничего не будем :(\n" );
            break;
        case 8:
            printf( "Сqweqweегодня смотреть ничего не будем :(\n" );
            break;
        case 9:
            printf( "Сегодняwdefqwf смотреть ничего не будем :(\n" );
            break;
        default:
            printf( "Неправильный вwfwvgfвод.\n" );
    }
    getchar();
    return 0;
}

