//
// Created by Stephane Kitsch on 10/1/21.
//
// AF_INET, SOCK_STREAM
#include <iostream>
#include <unistd.h>
#include <time.h>
#include <arpa/inet.h> // struct sockaddr_in
//#include <sys/socket.h> //socket()

#define MAXLINE 4096
#define	LISTENQ	1024 /* максимальное кол-во клиентских соединений */
#define	SA	struct sockaddr

int main(int argc, char **argv) {
    int listenfd, connfd;
    struct sockaddr_in  servaddr;
    char buf[MAXLINE];
    time_t  ticks;

    if ((listenfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        std::cout << "Error socket" << std::endl;
        return 1;
    }
    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY); /* Задаем IP-адрес как INADDR_ANY, что позволяет серверу приниматьсоединение клиента на любом интерфейсе, если узел сервера имеет несколько интерфейсов */
    servaddr.sin_port = htons(13);  /* задаем порт */

    if (bind(listenfd, (SA *) &servaddr, sizeof(servaddr) ) < 0) { // связываем порт сервера с сокетом
        std::cout << "Error bind" << std::endl;
        return 1;
    }

    if (listen(listenfd, LISTENQ) < 0) { // listen - преобразует сокет в прослушиваемый
        std::cout << "Error listen" << std::endl;
        return 1;
    }

    for (;;) {
        if ((connfd = accept(listenfd, (SA *) NULL, NULL)) < 0) {
            std::cout << "Error accept" << std::endl;
            return 1;
        }
        ticks = time(NULL);
//        snprintf(buf, sizeof(buf), "%.24s\er\en", ctime(&ticks));
        snprintf(buf, sizeof(buf), "Закрыто");

        if (write(connfd, buf, strlen(buf)) < 0) {
            std::cout << "Error write" << std::endl;
            return 1;
        }

        if (close(connfd) < 0) {
            std::cout << "Error close" << std::endl;
            return 1;
        }
    }


//    sig()
//    while ()
//
//

}
