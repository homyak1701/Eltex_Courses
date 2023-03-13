#include <stdio.h>
#include <dlfcn.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "name_func.h"

int main(int argc, char *argV[]){

    //- для циклов;
    int i = 0;
    //- переменная shift указывает полный размер структуры по указателям;
    //- так как мы сразу выделяем 8 быйт, то запишем туда SIZE;
    int shift_lib = SIZE;
    int shift_func = SIZE;
    //- выделяем память под струкруту указателей библиотек;
    struct info_libs *point_user_dynamic_libs = (struct info_libs *)malloc(sizeof(struct info_libs));
        error_memory(point_user_dynamic_libs);
    //- выделяем память под структуру с информацией о функции библиотеки;
    point_user_dynamic_libs->all_func = (struct info_func *)malloc(sizeof(struct info_func));
        error_memory(point_user_dynamic_libs->all_func);
    int (*general_func)(struct info_func *);

    //- при наличии указанных пользователем библиотек 
    //открываем их и берем нужные фукнции;
    if(argc > 1){
        for(i = 0; i < argc; i++){
            point_user_dynamic_libs->point_lib + shift = dlopen(argV[i], RTLD_LAZY);
                error_open(point_user_dynamic_libs->point_lib);
            shift_lib += SIZE;
            point_user_dynamic_libs->point_lib = realloc(point_user_dynamic_libs, shift);
                error_memory(point_user_dynamic_libs->point_lib);
            //- возьмем общую функцию для всех библиотек, которая заполняет структру 
            //о функция библиотек;
            general_func = dlsym(point_user_dynamic_libs->point_lib, "return_name_func");
            (*general_func)(&shift_func, point_user_dynamic_libs->all_func);
        }   
    } else {
        printf("Не одной библиотеки не выбрано.\nНапишение при запуске программы те библ., что вам нужны. \nОзнакомиться с асортиментом можно в папке libs.\n");
    }

    //- закрываем все дескрипторы библиотек;
    for(i = 0; i < shift_lib; i+= SIZE){
        free(point_user_dynamic_libs);
        point_user_dynamic_libs+=i;
    }
    //- закрываем все указатели на функции;
    for(i = 0; i < shift_func; i+= SIZE){
        free(point_user_dynamic_libs->all_func);
        point_user_dynamic_libs->all_func+=i
    }
    
    return 0;
}