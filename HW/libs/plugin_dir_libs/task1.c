//Калькулятор (плагины загружаемые из папки)
#include <stdio.h>
#include <stdlib.h>
#include <dlfcn.h>
#include <malloc.h>
#include <string.h>
#include <sys/types.h>
#include <dirent.h>

int loadLibrary(void);
int closeLibrary(void **, int);
int loadMenuLibrary(void **, int);
int arithmeticOperations(char *, int (*)(int, int));
int file_select(const struct dirent *);

int file_select(const struct dirent *compare) {
    int sizeCompareText = strlen(compare->d_name);

    if ((compare->d_name[sizeCompareText - 1] == 'o') &&
        (compare->d_name[sizeCompareText - 2] == 's') &&
        (compare->d_name[sizeCompareText - 3] == '.'))
        return (1);
    else
        return (0);
}

int main(void) {
    loadLibrary();

    return 0;
}

int loadMenuLibrary(void **pluginLibrary, int countLibrary) {
    char *DescriptionMenu = NULL;
    char *functionName = NULL;
    void *functionExecution = NULL;
    int menuItem = 0;

    printf("\nВведите номер меню\n");
    while (menuItem != (countLibrary)) {
        for (int i = 0; i < countLibrary; i++) {
            printf("%i: ", i);
            DescriptionMenu = (char *)(dlsym(pluginLibrary[i], "functionDescriptionMenu"));
            printf("%s\n", DescriptionMenu);
        }
        printf("Для выхода введите %d\n", (countLibrary));
        printf("Ввод: ");
        scanf("%d", &menuItem);
        system("clear");
        if ((menuItem >= 0) && (menuItem < countLibrary)) {
            DescriptionMenu = (char *)(dlsym(pluginLibrary[menuItem], "functionDescription"));
            functionName = (char *)(dlsym(pluginLibrary[menuItem], "functionName"));
            functionExecution = dlsym(pluginLibrary[menuItem], functionName);
            arithmeticOperations(DescriptionMenu, functionExecution);
        }

    }

    return 0;
}

int loadLibrary(void) {
    struct dirent **namelist = NULL;
    int countLibrary = 0;

    countLibrary = scandir("./Plugins", &namelist, file_select, alphasort);
    if (countLibrary < 0) {
        perror("scandir");
        exit(-1);
    }

    void **pluginLibrary = malloc(sizeof(void *) * countLibrary);
    if (pluginLibrary == NULL) {
        printf("ERROR");
        exit(-1);
    }

    for (int i = 0; i < countLibrary; i++) {
        pluginLibrary[i] = dlopen(namelist[i]->d_name, RTLD_LAZY);
        if (!(pluginLibrary[i])) {
            fprintf(stderr, "dlopen() error: %s\n", dlerror());
            exit(-1);
        };
    }

    loadMenuLibrary(pluginLibrary, countLibrary);

    closeLibrary(pluginLibrary, countLibrary);

    while (countLibrary--) {
        printf("%s\n", namelist[countLibrary]->d_name);
        free(namelist[countLibrary]);
    }
    free(namelist);

    return 0;
}

int closeLibrary(void **pluginLibrary, int countLibrary) {
    for (int i = 0; i < countLibrary; i++) {
        dlclose(pluginLibrary[i]);
    }

    free(pluginLibrary);

    return 0;
}

int arithmeticOperations(char *example, int (*operation)(int, int)) {
    int firstNumber;
    int secondNumber;
    int result;

    system("clear");
    printf("\n%s\n", example);
    printf("\nВведите a и b: ");
    scanf("%d %d", &firstNumber, &secondNumber );
    result = operation(firstNumber, secondNumber);
    printf("\nРезультат: %d\n\n", result);

    return 0;
}
