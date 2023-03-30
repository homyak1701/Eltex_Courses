#ifndef DOP_FUNC_H
#define DOP_FUNC_H

#define error_dir(a) do{if(a < 0){printf("line:%d\n", __LINE__); \
                                        perror("dir"); exit(EXIT_FAILURE);}} while(0)
#define error_info(a) do{if(0 != a){printf("line:%d\n", __LINE__); \
                                       perror("file"); exit(EXIT_FAILURE);}} while(0)
#define error_stat(a) do{if(a != 0){printf("line:%d\n", __LINE__); \
                                        perror("stat"); exit(EXIT_FAILURE);}} while(0)

//- функция для отображения данных о директории в одном из окон;
void see_wnd_dir(
                int shift_y, 
                int num_files, 
                WINDOW *wnd_names, 
                WINDOW *wnd_sizes, 
                WINDOW *wnd_mod, 
                WINDOW *wnd, 
                struct dirent **namelist
            );

//- смещение курсора;
void put(
            WINDOW *wnd_names, 
            WINDOW *wnd_sizes, 
            WINDOW *wnd_mod, 
            WINDOW *wnd, 
            int put,
            struct dirent *namelist,
            num_files
        );

#endif
