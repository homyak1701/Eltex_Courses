#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define error_file(a) do{if(a == -1){printf("line:%d\n", __LINE__); \
                                        perror("file"); exit(EXIT_FAILURE);}} while(0)
#define error_info(a) do{if(0 != a){printf("line:%d\n", __LINE__); \
                                       perror("file"); exit(EXIT_FAILURE);}} while(0)

int main(){

    int fd;
    char *file_name = "example.txt";
    char *data = "Hello World!\n";
    char *buf_read;

    //- структура с информацией о файле;
    struct stat file_stat;
    int result;
    int i = 0;

    //- для кареток;
    off_t start_cursor = 0;
    off_t curr_cursor = 0;

    //- сколько прочитали или записали байт;
    int amount_byte = 0;

    //S_IRWXU - права суперпользователя;
    fd = open(file_name, O_CREAT | O_RDWR, S_IRWXU);
        error_file(fd);

    amount_byte = write(fd, data, 11);
        error_file(amount_byte);

    printf("-----> %d (байт записали в файл)\n", amount_byte);

    close(fd);
        error_file(fd);

    fd = open(file_name, O_RDWR, S_IRWXU);
        error_file(fd);

    result = stat(file_name, &file_stat);
    buf_read = malloc(strlen(data));

    amount_byte = read(fd, (char *)buf_read, strlen(data));
        error_file(amount_byte);

    printf("%s\n", buf_read);

    start_cursor = lseek(fd, 0, SEEK_SET);
    curr_cursor = lseek(fd, 0, SEEK_END) - 1;

    while(-1 != curr_cursor){
        curr_cursor = lseek(fd, curr_cursor, SEEK_SET);
        
        amount_byte = read(fd, (char *)buf_read + i, 1);
            error_file(amount_byte);
        
        curr_cursor--; 
        i++;
    }

    printf("%s\n", buf_read);   

    close(fd);
    free(buf_read);

    return 0;
}