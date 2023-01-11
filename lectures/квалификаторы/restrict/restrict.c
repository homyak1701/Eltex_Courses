#include <stdio.h>

void update(int *restrict ptr1, int *restrict ptr2, int *restrict val)
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
