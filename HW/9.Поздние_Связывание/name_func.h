#include <stdio.h>

#ifndef NAME_FUNC_H
#define NAME_FUNC_H

#define error_memory(a) do{if(NULL == a){  printf("line:%d\n", __LINE__); \
                                        perror("memory"); exit(EXIT_FAILURE);}} while(0)
#define error_open(a) do{if(NULL == a){ printf("line:%d\n", __LINE__); \
                                        printf("Error opening library %s: %s\n", argV[i], dlerror()); \
                                        exit(EXIT_FAILURE);}} while(0)
#define error_sym(a) do{if(NULL == a){ printf("line:%d\n", __LINE__); \
                                        printf("Error opening sym %s: %s\n", argV[i], dlerror()); \
                                        exit(EXIT_FAILURE);}} while(0)
#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("name_func"); exit(EXIT_FAILURE);}} while(0)

#define SIZE 8

struct info_func{
    int ordinal_number; //- порядковый номер функции в программе;
    char *name_func; //- имя фукнции из библиотеки;
    char *description; //- что будет видеть пользователь при выборе функционала;
    int (*point_func)(); //- указатель на загруженную фукнцию;
};

struct info_libs{
    void *point_lib;
    struct info_func **all_func;
};

#endif 