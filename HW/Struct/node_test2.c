#include <stdlib.h>
#include <stdio.h>


//новый тип данных 
//sizeof = 24 байта
struct s_list {
	int id;
	char *name;

	struct s_list *next; // - переменная next содержит ссылку следующую на структуру
			     // - это называется указатель на структуру того же типа
			     // - в дальнейшем можно будет обращаться к переменным
			     // структуры через стрелку;
};

// *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** ---
// функция create_node возвращает ссылку на начало узла списка
// 1) просто число (id);
// 2) сслыку на область записи
struct s_list *create_node(int set_id, char *set_name) {

	//переменная node содержит ссылку на начало выделенной области
	struct s_list *node = (struct s_list* )malloc(sizeof(struct s_list)); //- создаем область узла
								       //и в переменную заносим
								       //ссылку на ее начало
	// область данных
	node -> id = set_id;
	node -> name = set_name;
	// сслыка на следующий узел
	node -> next = NULL;

	return node; // так как функция является указательной, то возвращается ссылка на начало списка;
}
// *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** --- *** ---
// функция push_front принимает 3 аргумента
// 1) указатель на связной список;
// 2-3) set_id, set_node нужны для вызова вверхней функции;

void push_front(struct s_list **list, int set_id, char *set_name)
{
	struct s_list *new_element = create_node(set_id, set_name); // переменная содержит в себе ссылку
							            //
	new_element -> next = *list;
	*list = new_element;
}


int main(void) {
	struct s_list *list = create_node(0, "абонент_1"); // переменная содержит ссыку на начало списка
	
	printf("id=%d name=%s\n", list -> id, list -> name);

	if(list -> next == NULL)
		printf("<list -> next == NULL>\n");
	
	free(list);
	return 0;
}
