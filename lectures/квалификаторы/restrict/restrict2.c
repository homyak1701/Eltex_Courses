#include <stdio.h>

void update(int *ptr1, int *ptr2, int *val)
{
    *ptr1 += *val;
    *ptr2 += *val;
}

int main(void)
{
    int a = 5;
    int b = 7;
    int c = 9;

    update(&a, &b, &c);

    return 0;
}

