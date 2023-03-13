#ifndef NAME_FUNC_H
#define NAME_FUNC_H

#define error_memory(a) do{if(NULL == a){  printf("line:%d\n", __LINE__); \
                                        perror("memory"); exit(EXIT_FAILURE);}} while(0)
#define error_open(a) do{if(NULL == a){ printf("line:%d\n", __LINE__); \
                                        perror("open_dir"); exit(EXIT_FAILURE);}} while(0)

#define SIZE 8

struct info_libs{
    char *name_lib;
    void *point_lib;
    struct info_func *all_func;
};

struct info_func{
    char name_func; //- имя фукнции из библиотеки;
    void (*point_func)(int a, int b); //- указатель на загруженную фукнцию;
};

int return_name_func(void *point_lib, struct info_func *point_func);

#endif 