#include <stdio.h>
#include <dlfcn.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 8


#define error_memory(a) do{if(NULL == a){  printf("line:%d\n", __LINE__); \
                                        perror("memory"); exit(EXIT_FAILURE);}} while(0)
#define error_open(a) do{if(NULL == a){ printf("line:%d\n", __LINE__); \
                                        perror("open_dir"); exit(EXIT_FAILURE);}} while(0)

struct info_func{
    void *shoft_to_point_func;
    void (*point_func)(int a, int b);
};

int main(int argc, char *argV[]){
    
    // Для циклов;
    int i = 0;
    //- Хранит сдвиг для указателя библиотеки
    //- Так как мы сразу выделяем 8 быйт, то запишем туда SIZE;
    int shift = SIZE;
    //- указатель для загрузки библиотеки;
    //- будет указателем начала череды указателей;
    void *handle = malloc(SIZE);
        error_memory(handle);
    //- получаем указатель на структуру об информации библиотеки;
    struct info_func *info_func = (struct info_func *)malloc(sizeof(struct info_func));
        error_memory(info_func);

    if(argc > 1){
        for(i = 0; i < argc; i++){
            handle = dlopen(argV[1], RTLD_LAZY);
                error_open(handle);
            shift += SIZE;
            handle = realloc(handle,shift);

                error_memory(handle);
        }   
    } else {
        printf("Не одной библиотеки не выбрано.\nНапишение при запуске программы те библ., что вам нужны. \nОзнакомиться с асортиментом можно в папке libs.\n");
    }

    return 0;
}

