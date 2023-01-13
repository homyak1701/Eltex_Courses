#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Составить список с абонентами:

// 1)Добавить абонента;
// 2)Список абонентов;
// 3)Поиск абонента по имени;
// 4)Удалить абонента;
// 5)выйти из программы;

struct user
{
	char number[20];
	char name[20];
	char sec_name[20];
	int id_chel;
};


int main(void)
{
	struct user chel[9];
	char chel_serch[20];
	int schet = 0;
	int io = 0;
	int i = 0;
	int y = 0;


	for(i = 0; i < 10; i++)
	{
		chel[i].id_chel = 0;
		for(y = 0; y < 20; y++)
		{
			chel[i].name[y] = 0;
			chel[i].sec_name[y] = 0;
			chel[i].number[y] = 0;
		}
	}

	printf("\n\n--- Добро пожаловать в список абонентов ---");
	printf("\n\n\nЧтобы продолжить выберите один из нижеперечисленных пунктов и введите его значение \n");
	while(1)
	{
		printf(" Чего изволите:\n 1.Добавить абонента;\n 2.Показать список абонентов;\n 3.Поиск абонента по имени;\n 4.Удалить абонента;\n 5.Выход из программы \n\n Ввод--> ");
		scanf("%d", &io);
		switch (io)
		{
			case 1:
				for (i = 0; i < 10; i++)
				{
					if(0 == chel[i].id_chel)
					{
						printf("\n\nВведите имя (максимум 20 символов);\n Ввод--> ");
						//fgets(chel[i].name, sizeof(chel[i].name), stdin);
						scanf("%s",chel[i].name); //читает символы, пока не встретит разделитель;
						printf("\nВведите фамилию (максимум 20 символов);\n Ввод--> ");
						//fgets(chel[i].sec_name, sizeof(chel[i].sec_name), stdin);
						scanf("%s",chel[i].sec_name);
						printf("\nВведите номер абонента (сотовый номер из 11 символов);\n Ввод--> ");
						//fgets(chel[i].number, sizeof(chel[i].number), stdin);
						scanf("%s",chel[i].number);
						chel[i].id_chel = 1;
						i = 10;
					}
				}
				printf("\nАбонент записан\n\n");
				break;
			case 2:
				schet = 1;
				for (i = 0; i < 10; i++)
                                {
                                        if(1 == chel[i].id_chel)
                                        {
                                                printf("\n\n %d) %s %s --  ",i,chel[i].name,chel[i].sec_name);
                                                printf("%s \n*****\n", chel[i].number);
                                        }
                                }
                                break;
			case 3:
				printf("\nВведите имя пользователя которого хотите найти\n Ввод --> ");
				scanf("%s",chel_serch);
				for (i = 0; i < 10; i++)
                                {
					if(!(strcmp(chel[i].name, chel_serch)))
					{
						printf("\n\n %d) %s %s --  ",schet++,chel[i].name,chel[i].sec_name);
                                                printf("%s \n*****\n", chel[i].number);
					}
                                }
				break;
			case 4:
				printf("\nВведите имя пользователя которого хотите удалить\n Ввод --> ");
                                scanf("%s",chel_serch);
				for(i = 0; i < 10; i++)
				{
                                        if(!(strcmp(chel[i].name, chel_serch)))
                                        {
						chel[i].id_chel = 0;
                				for(y = 0; y < 20; y++)
                				{
                        				chel[i].name[y] = 0;
                        				chel[i].sec_name[y] = 0;
                        				chel[i].number[y] = 0;
                				}
                                        }
                                }
				printf("\n\n Well done \n\n");
				break;
			case 5:
				printf("Приятного пути. До свидания.\n");
				exit (0);
			default:
				printf("То, что вы ввели не иммет смысла. Введите число из перечисленных вариатов\n\n");
				break;
		}
	}
	return 0;
}
