static pthread_key_t key; //переменная для создания ключа
static pthread_once_t key_once = PTHREAD_ONCE_INIT; //переменная для использвания
						    //первым потоком функции единожды 

static void
make_key()
{
	/*создание ключа*/
	/*и передача по глобальной ссылки*/
	(void) pthread_key_create(&key, NULL);
}

func()
{
	/*создаем указатель*/
	void *ptr;
	
	/*функция единождого использования функции потоком*/
	(void) pthread_once(&key_once, make_key);
	
	/*Функция pthread_getspecific() должна возвращать значение, 
	привязанное в данный момент к указанному ключу от имени вызывающего потока.
	в нашем случае это динамическая память*/	
	if ((ptr = pthread_getspecific(key)) == NULL) {
		ptr = malloc(OBJECT_SIZE);
		...
		/*связываем ключ потока со значением*/
		(void) pthread_setspecific(key, ptr);	
	}
        
	...
}
