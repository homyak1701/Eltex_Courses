#include <stdio.h>

int main(){

    int a = 0x00DDAABB;
    int a_part = 0;
    int counter = 0;

    for(counter = 0; counter != sizeof(int); counter++){
        a_part = a & (0x000000FF);
        a=a>>8;
        printf("---------\n");
        printf("--> 0x%x\n", a_part);
        printf("--> %d\n", a_part);
        printf("---------\n");
    }

    return 0;
}