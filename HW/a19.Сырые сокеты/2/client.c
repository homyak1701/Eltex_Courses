#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/udp.h>
#include <netinet/ip.h>


#define error_func(a) do{if(-1 == a){ printf("line:%d\n", __LINE__); \
                                        perror("error"); exit(EXIT_FAILURE);}} while(0)

int main(void){

    struct sockaddr_in serv, client;
    //- дескриптор сокета;
    int fd_serv, fd_client;
    //- проверка возвращаемых функций;
    int status;
    ssize_t amount_byte;
    //- заголовок udp;
    struct udphdr *head_udp;
    //- заголовок ip
    struct ip *head_ip;
    //- для фукнкций sendto() и recvfrom();
    socklen_t len;
    len = sizeof(client);
    //- буфер, который бдует хранить отправленные и принимаемые
    //пакеты;wd
    char buffer[200];
    //- указатель, который нам позволит перемещаться по пакету;
    char *mover;
    //- включенное значение опции;
    int opt = 1;

    fd_serv = socket(AF_INET, SOCK_RAW, IPPROTO_UDP);
    status = setsockopt(fd_serv, IPPROTO_IP, IP_HDRINCL, &opt, sizeof(opt));
        error_func(status);

    //- заполняем адрес отправления;
    serv.sin_family = AF_INET;
    serv.sin_port = htons(3457);
    serv.sin_addr.s_addr = inet_addr("127.0.0.1");

    while(1){

        sleep(1);
        memset(buffer, 0, sizeof(buffer));

        //- переместимся за заголовок к данным;
        mover = buffer + sizeof(struct udphdr) + sizeof(struct ip);

        printf("Напишем сереверу: ");
        fgets(mover, 100, stdin);
        mover[strcspn(mover, "\n")] = '\0';

        //- получаем указатель на ip заголовок;
        head_ip = (struct ip*)buffer;
        //- заполняем ip заголовок;
        head_ip->ip_hl = 5;
        head_ip->ip_v = 4;
        head_ip->ip_tos = 1;
        //head_ip->ip_len = htons(sizeof(struct ip) + sizeof(struct udphdr) + strlen(mover));
        head_ip->ip_id = htons(1);
        head_ip->ip_off = 0;
        head_ip->ip_ttl = htons(10);
        head_ip->ip_p = IPPROTO_UDP;
        //head_ip->ip_sum = 0;
        head_ip->ip_src.s_addr = inet_addr("127.0.0.1");
        head_ip->ip_dst.s_addr = inet_addr("127.0.0.1");

        //- получим указатель на заголовок udp;
        head_udp = (struct udphdr*)(buffer + sizeof(struct ip));
        //- заполним udp заголовок;
        head_udp->source = ntohs(1456);
        head_udp->dest = ntohs(3457);
        head_udp->len = ntohs(sizeof(struct udphdr) + strlen(mover));
        head_udp->check = 0;

        amount_byte = sendto(fd_serv, (void *)buffer, sizeof(struct ip) + sizeof(struct udphdr) + strlen(mover) + 1, 0, (struct sockaddr*)&serv, len);
            error_func(amount_byte);

        if(!strcmp(mover,"exit")){
            break;
        }

        while(1){

            //- ждем ответ от сервера;
            amount_byte = recvfrom(fd_serv, (void *)&buffer, sizeof(buffer), 0, (struct sockaddr*)&serv, &len);
            error_func(amount_byte);
        
            if(0 == amount_byte){
                break;
            } else if (3457 == ntohs(head_udp->source)){
                //- проверяем, что ответ пришел от сервера;
                break;
            }
            
        }

        printf("Сервер отвечает: %s\n", mover);
    }

    close(fd_serv);

    exit(EXIT_SUCCESS);
}