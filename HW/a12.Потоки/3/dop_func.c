#include <pthread.h>
#include <string.h>
#include <stdlib.h>

#define MAX_ERR_NUM 9
#define MAX_ERR_STR_LEN 20
#define MAX_STRERROR_LEN 256

static pthread_key_t strerror_key;
static pthread_once_t key_once = PTHREAD_ONCE_INIT;

static char err_buf[MAX_ERR_NUM + 1][MAX_ERR_STR_LEN];

char *my_strerror(int err_num) {
    if (err_num < 0 || err_num > MAX_ERR_NUM) {
        return "Unknown error";
    }

    switch (err_num) {
        case 0:
            return "No error";
        case 1:
            return "Error file";
        case 2:
            return "Error lib";
        case 3:
            return "Error key";
        case 4:
            return "Error point";
        case 5:
            return "Error stack";
        case 6:
            return "Error heap";
        case 7:
            return "Error bss";
        case 8:
            return "Error data";
        case 9:
            return "Error text";
        default:
            return "Unknown error";
    }
}

static void create_pthread_key() {
    pthread_key_create(&strerror_key, NULL);
}

char *my_strerror_r(int err_num, char *buf, size_t buflen) {
    char *strerror_buf;
    int strerror_len;

    pthread_once(&key_once, create_pthread_key);

    if ((strerror_buf = pthread_getspecific(strerror_key)) == NULL) {
        strerror_buf = malloc(MAX_STRERROR_LEN);
        pthread_setspecific(strerror_key, strerror_buf);
    }

    strncpy(strerror_buf, my_strerror(err_num), MAX_STRERROR_LEN);
    strerror_buf[MAX_STRERROR_LEN - 1] = '\0';
    strerror_len = strlen(strerror_buf);

    if (strerror_len >= buflen) {
        strncpy(buf, strerror_buf, buflen - 1);
        buf[buflen - 1] = '\0';
        return buf;
    } else {
        strcpy(buf, strerror_buf);
        free(strerror_buf);
        return buf;
    }
}