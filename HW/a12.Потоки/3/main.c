#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <pthread.h>
#include <string.h>

#define NUM_THREADS 10

typedef char *(*strerror_r_func)(int, char *, size_t);

void *thread_func(void *arg) {
    int err_num = *((int *) arg);

    void *lib_handle = dlopen("./libdop_func.so", RTLD_LAZY);
    if (!lib_handle) {
        fprintf(stderr, "Error: %s\n", dlerror());
        exit(EXIT_FAILURE);
    }

    strerror_r_func strerror_r_ptr = (strerror_r_func)dlsym(lib_handle, "my_strerror_r");
    if (!strerror_r_ptr) {
        fprintf(stderr, "Error: %s\n", dlerror());
        dlclose(lib_handle);
        exit(EXIT_FAILURE);
    }

    char buf[256];
    memset(buf, 0, sizeof(buf));
    char *str_err = strerror_r_ptr(err_num, buf, sizeof(buf));

    printf("Thread %lu: Error %d: %s\n", pthread_self(), err_num, str_err);

    dlclose(lib_handle);

    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];
    int err_nums[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++) {
        err_nums[i] = i;
        int ret = pthread_create(&threads[i], NULL, thread_func, (void *) &err_nums[i]);
        if (ret != 0) {
            fprintf(stderr, "Error: pthread_create failed: %s\n", strerror(ret));
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    return 0;
}