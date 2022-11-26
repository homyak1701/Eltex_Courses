#include <stdio.h>
#include <stdlib.h>
 
void main() {
    const int maxNumber = 100;
    char *p = NULL;
    unsigned i, size;
 
    do {
        printf("Enter number from 0 to %d: ", maxNumber);
        scanf("%d", &size);
        if (size < maxNumber) {
            break;
        }
    } while (1);
 
    p = (char*) malloc(size * sizeof(char*));
 
    printf("\n\n----> sizeof(p[0]) = %ld\n\n",sizeof(p[0]));
    for (i = 0; i < size; i++) {
        p[i] = i*i;
    }
 
    for (i = 0; i < size; i++) {
        printf("%d ", p[i]);
    }
 
    free(p);
}
