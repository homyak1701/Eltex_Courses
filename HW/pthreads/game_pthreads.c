#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

/*макросы по обработке ошибок*/
#define handle_error_en(en, msg) \
	do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
	do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {    /*используется в качетсве аргумента для thread_start()*/
	pthread_t thread_id;/*ID, возвращаемый pthread_create()*/
	int thread_num;     /*Поток, определяемый приложением #*/
	char *argv_string;      /*из аргумента командной строки*/
};

/* Thread start function: display address near top of our stack,
	and return upper-cased copy of argv_string */

static void *thread_start(void *arg) 
{
	struct thread_info *tinfo = arg;
	char *uargv; 
	char *p;

	printf("Thread %d: top of stack near %p; argv_string=%s\n", tinfo->thread_num, &p, tinfo->argv_string);
	uargv = strdup(tinfo->argv_string);
        if (uargv == NULL)
        	handle_error("strdup");

        for (p = uargv; *p != '\0'; p++)
        	*p = toupper(*p);

        return uargv;
}

int main(int argc, char *argv[])  /*1) количетсво арг, 2) какие аргументы*/
{
	int s;/*переменная для обработки правильной инициализации атрибутов потока*/
	int tnum; /*номер структуры для элемента, который вводим*/
	int opt;/*переменная в которую будут записываться параметры командной строки*/
	int num_threads; /*номер потоков*/
	struct thread_info *tinfo;/*для потоков*/
	pthread_attr_t attr;
	int stack_size;
	void *res;

           /* Параметр "-s" указывает размер стека для наших потоков */

        stack_size = -1;
	/*принимаем параметры из командной строки*/
        while ((opt = getopt(argc, argv, "s:")) != -1) { 
        switch (opt) {
        	case 's':
			/*преобразование строку, которая пришла 
			в качетсве аргумента в число
			(строка бкдет идти после аргумента)*/
                	stack_size = strtoul(optarg, NULL, 0);
                	break;

               default:
                   fprintf(stderr, "Usage: %s [-s stack-size] arg...\n", argv[0]);
                   exit(EXIT_FAILURE);
               }
           }

           num_threads = argc - optind; /*номер элемента массива ввода - индекск след. 
					  			обрабатываемого элемента argv
							то есть получется введенный слов без параметров*/

           /* Initialize thread creation attributes */

           s = pthread_attr_init(&attr); /*инициализируем атрибуты потоков*/
           if (s != 0)
               handle_error_en(s, "pthread_attr_init");

           if (stack_size > 0) {
               s = pthread_attr_setstacksize(&attr, stack_size); /*изменяем размер буфера потора
	       								в зависимости от значения параметра
											который введет пользователь*/
               if (s != 0)
                   handle_error_en(s, "pthread_attr_setstacksize");
           }

           /* выделить память для аргументов pthread_create() */

           tinfo = calloc(num_threads, sizeof(struct thread_info));
           if (tinfo == NULL)
               handle_error("calloc");

           /* создаем один поток для каждого аргумента командной строки */

           for (tnum = 0; tnum < num_threads; tnum++) {
               tinfo[tnum].thread_num = tnum + 1;
               tinfo[tnum].argv_string = argv[optind + tnum];

               /* Вызов pthread_create() сохраняет идентификатор потока в соответствующий элемент tinfo[] */
		/* в качетсве функции исполнения передается ссылка на функцию thread_start */
               s = pthread_create(&tinfo[tnum].thread_id, &attr,
                                  &thread_start, &tinfo[tnum]);
               if (s != 0)
                   handle_error_en(s, "pthread_create");
           }

           /* Уничтожить объект атрибутов потока, так как он больше не нужен */

           s = pthread_attr_destroy(&attr);
           if (s != 0)
               handle_error_en(s, "pthread_attr_destroy");

           /* Now join with each thread, and display its returned value */

           for (tnum = 0; tnum < num_threads; tnum++) {
               s = pthread_join(tinfo[tnum].thread_id, &res);
               if (s != 0)
                   handle_error_en(s, "pthread_join");

               printf("Joined with thread %d; returned value was %s\n",
                       tinfo[tnum].thread_num, (char *) res);
               free(res);      /* Free memory allocated by thread */
           }

           free(tinfo);
           exit(EXIT_SUCCESS);
       }

