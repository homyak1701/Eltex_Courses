#include <stdlib.h>
#include <stdio.h>

/*
// прототип списка
// внутри структуры мне нужно иметь тип указателя на данное же звено
typedef struct Node {
	int value; //хранимое значение
	struct Node *next; //указатель на следующие звено;
			   //зачему мы закольцовываем?
} Node;

Node *head = NULL; // первый элемент списка

//создание нового узла
void push(Node **head, int data)
{
	// 1) Выделяем ему память
	Node *tmp = malloc(sizeof(Node)); // указатель на начало выделенного узла
	// 2) Присваиваем ему значение
	tmp -> value = data;
	// 3) Присваиваем указателю tmp адрес предыдущего узла
	tmp -> next  = (*head);
	// 4) Присваиваем указателю head адрес нового узла
	(*head) = tmp;
} 

//функция которая удаляет элемент, на который указывает head
int pop(Node **head)
{
	Node *prev = NULL;
	int val;
	if (head == NULL)
	{
		exit(-1);
	}
	prev = (*head);
	val = prev -> value;
	(*head) = (*head) -> next;
	free(prev);
	return val;
}

*/


int main (void)
{
	int set_name = "Name1";
	printf("\nset_name = %s\n", (char*)set_name);
	//printf("\n*set_name = %d\n", *set_name);
	//printf("\n&set_name = %p\n", &set_name);
}





