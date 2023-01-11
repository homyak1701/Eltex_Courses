#include <stdio.h>

int main(){

    volatile int a = 15;
    volatile int b = 0;

    b++;
    b++;

    if (15 != a){
        printf("a != 15");
    } else {
        printf("a = 15");
    }
    return 0;
}
