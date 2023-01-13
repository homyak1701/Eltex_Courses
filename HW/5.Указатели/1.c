#include <stdio.h>

int main(){

    //- число из 4 байтов;
    int a = 270533154;
    //- указатель на один байт, с помощью
    //которого будем просматривать побайтно a;
    char *one_byte_pointer = (char *)&a;

    printf("--> a = 0x%x\n", a);
    printf("--> a = 0x%x\n", *one_byte_pointer);

    one_byte_pointer++;

    printf("--> a = 0x%x\n", a);
    printf("--> a = 0x%x\n", *one_byte_pointer);

    one_byte_pointer++;

    //- разыменуем третий байт и изменим его значение;

    *one_byte_pointer = 66;

    printf("--> a = 0x%x\n", a);
    printf("--> a = 0x%x\n", *one_byte_pointer);

    return 0;
}