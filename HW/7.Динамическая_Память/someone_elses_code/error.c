#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_BUFFER_SIZE 5

//- эта фукнция нужна для...
char *get_unlimited_input(char* buffer);

int main() {
    // printf(">");
    // char *buffer = get_unlimited_input(NULL);
    // printf(">");
    // get_unlimited_input(buffer);

    char a = 1;
    char b = 2;

    char *p_a = &a;
    char *p_b = &b;

    printf("---> p_a - p_b = %ld\n", p_a+p_b);
}

char *get_unlimited_input(char *buffer) {
    //- текущий размер в байтах
    size_t current_size;
    //- проверка сущетсвует ли буфер со строкой
    //которая передается в качетсве параметра;
    if (buffer == NULL) {
        //- если NULL выделяем 5 байт;
        buffer = malloc(DEFAULT_BUFFER_SIZE);
        //- текущий размер буфера;
        current_size = DEFAULT_BUFFER_SIZE;
    } else {
        //- определяем длину строки и прибавляем 5 байт;
        //- тут мы определяем выделенный размер буфера;
        current_size = strlen(buffer) + DEFAULT_BUFFER_SIZE;
    }
    //- сдвигаем указатель на текущий размер строки;
    //то есть побайтно;
    char *cursor = buffer + current_size - DEFAULT_BUFFER_SIZE;
    //
    for (;;) {
        int current = getchar();
        //- сначала проверяем что за символ пришел
        //вдруг это конец;
        if (current == '\n' || current == EOF)
            break;

        *cursor = (char)current;
        cursor++;

        if (cursor >= buffer + current_size) {
            current_size += DEFAULT_BUFFER_SIZE;
            buffer = realloc(buffer, current_size);
            cursor = buffer + current_size - DEFAULT_BUFFER_SIZE;
        }
    }
    *cursor = '\0';
    buffer = realloc(buffer, cursor - buffer);
    return buffer;
}