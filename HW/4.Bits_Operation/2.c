#include <stdio.h>

int main(){

    int a = 0x00DDAABB;

    a = a & 0x00BBAABB;

    printf("---------\n");
    printf("--> 0x%x\n", a);
    printf("---------\n");

    return 0;
}