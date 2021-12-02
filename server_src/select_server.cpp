//
// Created by Stephane Kitsch on 11/18/21.
//

#include "../utils_src/net_write_read.hpp"

#include <sys/select.h> /* FD_SETSIZE */
#include <sys/types.h> /* ssize_t fd_set */
//#include <sys/socket.h> /* socklen_t - непрозрачный целочисленный тип без знака длиной не менее 32 бит.*/
#include <netinet/in.h>	/* sockaddr_in{} and other Internet defns */
#include <netdb.h> /* perror(x) */

#include <unistd.h>
#include <strings.h>
#include <iostream>

#define	MAXLINE		4096	/* max text line length */
#define	SERV_PORT   9877	/* TCP and UDP */

int main(int argc, char **argv) {
    int i, maxi, maxfd, listenfd, connfd, sockfd;
    int nready, client[FD_SETSIZE];
    ssize_t n;
    fd_set rset, allset;
    char buf[MAXLINE];
    socklen_t addrlen, len /*clilen*/;
    struct sockaddr *cliaddr;

    if (argc == 2)
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    else if (argc == 3)
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    else {
        std::cout << "err argc" << std::endl;
        return 1;
    }

    if ( (cliaddr = (struct sockaddr *)malloc(addrlen)) == NULL) {
        std::cout << "err malloc" << std::endl;
        return 1;
    }

    maxfd = listenfd;   /* инициализация */
    maxi = -1;  /* индекс в массиве client[] */
    for (i = 0; i < FD_SETSIZE; i++) {
        client[i] = -1;     /* -1 означает свободный элемент */
    }

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);      /* единственный дескриптор, который мы изначально выберем с помощью select - это
                                        * это прослушиваемый сокет */
    for (;;) {
        rset = allset;
        if ((nready = select(maxfd + 1, &rset, NULL, NULL, NULL)) < 0) {
            err_sys("Error select\n");
        }

        if (FD_ISSET(listenfd, &rset)) { /* соединение с новым клиентом */
            len = addrlen;
            if ((connfd = accept(listenfd, cliaddr, &len)) < 0) {
                err_sys("Error accept\n");
            }
            std::cout << "New connect: " << connfd << std::endl;

            for (i = 0; i < FD_SETSIZE; i++) { /* FD_SETSIZE - константа с максимальным кол-ом дескрипторов для rset */
                if (client[i] < 0) {
                    client[i] = connfd; /* сохряняем дескриптор */
                    break;
                }
            }
            if (i == FD_SETSIZE)
                err_sys("too many clients");
            FD_SET(connfd, &allset);    /* добавление нового дескриптора */
            if (connfd > maxfd)
                maxfd = connfd;     /* для ф-ции select */
            if (i > maxi)
                maxi = i;   /* максимальный индекс в массиве client[] */
            if (--nready <= 0 )
                continue;   /* больше нет дескрипторов, готовых для чтения */
        }

        for (i = 0; i <= maxi; i++) {   /* проверяем все клиенты на наличие данных */
            if ( (sockfd = client[i]) < 0)
                continue;
            if (FD_ISSET(sockfd, &rset)) {
                if ( (n = read(sockfd, buf, MAXLINE)) == 0) {
                    /* Соединение закрыто клиентом */
                    close(sockfd);
                    FD_CLR(sockfd, &allset);
                    client[i] = -1;
                } else {
                    std::cout << "Sent message to: " << sockfd << std::endl;
                    if (writen(sockfd, buf, n) < 0)
                        err_sys("Error writen\n");
                }

                if (--nready <= 0)
                    break;      /* больше нет дескрипторов, готовых для чтения */
            }
        }
    }


}