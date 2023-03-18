#include <stdio.h>
#include <dlfcn.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>

#include "name_func.h"

int main(int argc, char *argV[]){

    //- проверка на наличие библиотек;
    if(1 == argc){
        printf("Не одной библиотеки не выбрано.\nНапишение при запуске программы те библ., что вам нужны. \nОзнакомиться с асортиментом можно в папке libs.\n");
        exit(0);
    }

    //- для циклов;
    int i = 0, j = 0, q = 0, error = 0, seq = 1;
    //- переменная shift указывает полный размер структуры по указателям;
    //- так как мы сразу выделяем 8 быйт, то запишем туда SIZE;
    int shift_lib = SIZE;
    //int num_func = 0;
    int *amount_func = NULL;
    char **name_func_dlsym = NULL;
    char **desc_func_dlsym = NULL; 
    //- выделяем память под указатели массив переданных библиотек;
    struct info_libs **point_user_dynamic_libs = (struct info_libs **)malloc((argc - 1) * sizeof(struct info_libs));
        error_memory(point_user_dynamic_libs);
    //- выделяем память под каждую библиотеку;
    for(i = 0; i < argc - 1; i++){
        point_user_dynamic_libs[i] = (struct info_libs *)malloc(sizeof(struct info_libs));
            error_memory(point_user_dynamic_libs[i]);
    }

    //- при наличии указанных пользователем библиотек 
    //открываем их и берем нужные фукнции;
    for(i = 1; i < argc; i++){
        
        point_user_dynamic_libs[i-1]->point_lib = dlopen(argV[i], RTLD_LAZY);
            error_open(point_user_dynamic_libs[i-1]->point_lib);

        //- возьмем общую функцию для всех библиотек, которая заполняет структру 
        //о функция библиотек;
        amount_func = dlsym(point_user_dynamic_libs[i-1]->point_lib, "AMOUNT");
            error_open(amount_func);

        //- выделяем место для функций из библиотеки;
        point_user_dynamic_libs[i-1]->all_func = (struct info_func **)malloc(*(amount_func) * sizeof(struct info_func *));
        
        name_func_dlsym = dlsym(point_user_dynamic_libs[i - 1]->point_lib, "name_funcs");
            error_sym(name_func_dlsym);
        desc_func_dlsym = dlsym(point_user_dynamic_libs[i - 1]->point_lib, "description");
            error_sym(desc_func_dlsym);

        //- перебираем все функции;
        for(j = 0; j < *amount_func; j++){
            //- выделяем место под фукнцию;
            point_user_dynamic_libs[i - 1]->all_func[j] = (struct info_func *)malloc(sizeof(struct info_func));
                error_memory(point_user_dynamic_libs[i - 1]->all_func[j]);
            //- записываем имя фукнции в структуру;
            point_user_dynamic_libs[i - 1]->all_func[j]->name_func = name_func_dlsym[j];
            //- записываем отображение для пользователя в структуру;
            point_user_dynamic_libs[i - 1]->all_func[j]->description = desc_func_dlsym[j];
            //- берем указатель на фукнцию;
            point_user_dynamic_libs[i - 1]->all_func[j]->point_func=dlsym( \
                                                            point_user_dynamic_libs[i - 1]->point_lib,  \
                                                            point_user_dynamic_libs[i - 1]->all_func[j]->name_func \
                                                                         );
                error_sym(point_user_dynamic_libs[i - 1]->all_func[j]->point_func);
            //- записываем порядковый номер фукнции;
            point_user_dynamic_libs[i - 1]->all_func[j]->ordinal_number = seq;
                seq += 1;
        }
    }   

    //- начинаем работу с пользователем;    
    printf("****\nДобро пожаловать!\n****\n****\n\n");

    while(1){

        printf("\n");
        printf("Выберите действие");
        printf("\n");

        //- послужит нам для нумерации;
        q = 1;

        //- выведем все функции, которые нам доступны;
        for(i = 0; i < argc-1; i++){
            amount_func = dlsym(point_user_dynamic_libs[i]->point_lib, "AMOUNT");
            for(j = 0; j < *amount_func; j++){
                printf("%d) %s;\n", q, point_user_dynamic_libs[i]->all_func[j]->description);
                q++;
            }
        }
        printf("press x to exit");

        //- пользователь выберает порядковый номер функции;
        printf("\n");
        printf("---> ");
        scanf("%d", &q);
        printf("\n");

        //- вызываем выбранную фукнцию;
        for(i = 0; i < argc-1; i++){
            amount_func = dlsym(point_user_dynamic_libs[i]->point_lib, "AMOUNT");
            for(j = 0; j < *amount_func; j++){
                if(q == point_user_dynamic_libs[i]->all_func[j]->ordinal_number){
                    point_user_dynamic_libs[i]->all_func[j]->point_func();
                }
            }
        }
    }

    for(i = 0; i < argc-1; i += 1){
        for(j = 0; j < *amount_func; j++){
            free(point_user_dynamic_libs[i]->all_func[j]);
        }
        dlclose(point_user_dynamic_libs[i]->point_lib);
        free(point_user_dynamic_libs[i]);
    }
    
    return 0;
}