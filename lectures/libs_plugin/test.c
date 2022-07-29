#include <stdio.h>
#include "my_func.h"

int main(void)
{
        int a = 10;

        a = my_func(a);
        printf("a = %d\n", a);

        return 0;
}

