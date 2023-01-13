/*создание очереди на интерфейсе system_v*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>

/*создадим структуру для сообщения и его приоритета*/
struct msfbuf{
    long mtype;
    char mtext[80];
};


int main(){

    const char*pathname = "/home/andrey/Рабочий стол/yest/Eltex_Courses/HW/очередь_сообщения/system_v";
    int proj_id = 1; /*параметр для создания уникального ключа*/
    int id_queue; /*идентификатор очереди*/
    int result; /*служит для проверки отправления сообщения*/
    key_t key_queue; /*ключ ipc system_v*/
    struct msfbuf msgbuf_send = {5, "Hello!\n"}; /*структура для хранения сообщения и его приоритета*/

    /*Функция ftok преобразовывает существующее полное имя 
        и целочисленный идентификатор в значение типа key_t (называемое ключом IPC — IPC key)*/
    /*По сути ключ - это имя очереди сообщения*/
    key_queue = ftok(pathname, proj_id); /*второй парамерт нужен для создания уникального ключа*/
    if(-1 == key_queue){
        perror("создание ключа");
        exit(EXIT_FAILURE);
    }

    /*плучаем идентификатор потока*/
    id_queue = msgget(key_queue, IPC_CREAT);
    if(-1 == id_queue){
        perror("идентификатор потора");
        exit(EXIT_FAILURE);
    }

    /*отправляем сообщение в буфер очереди*/
    result = msgsnd(id_queue, &msgbuf_send, 80, IPC_NOWAIT);
    if(-1 == result){
        perror("ошибка отправки сообщения\n");
        exit(EXIT_FAILURE);
    }

    sleep(10);
    exit(EXIT_SUCCESS);
}