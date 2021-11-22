//
// Created by Stephane Kitsch on 10/21/21.
//

#include <unistd.h>
#include <iostream>
#include <cstring>
#include <errno.h>
#include <stdio.h>
#include <netdb.h> /* struct addrinfo */
//#include <sys/socket.h>
#include "net_write_read.hpp"

void err_sys(const char* x) {
    perror(x);
    exit(1);
}

ssize_t writen(int fd, const void *vptr, size_t n) {
    size_t nleft;
    ssize_t nwritten;
    const char *ptr;

    ptr = (const char *)vptr;
    nleft = n;
    while (nleft > 0) {
        if ( (nwritten = write(fd, ptr, nleft)) <= 0) {
            if (errno == EINTR)
                nwritten = 0;
            else
                return -1;
        }
        nleft -= nwritten;
        ptr += nwritten;
    }
    return n;
}

/* срт. 720 */
int Tcp_listen(const char *host, const char *serv, socklen_t *addrlenp) {
    int listenfd, n;
    const int on = 1;
    struct addrinfo hints, *res, *ressave;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_flags = AI_PASSIVE;
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((n = getaddrinfo(host, serv, &hints, &res)) != 0) {
        std::cout << "Error getaddrinfo" << std::endl;
        return 1;
    }
    ressave = res;

    do {
        listenfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

        if (listenfd < 0)
            continue; /* ошибка, пробуем следующий адрес */

        if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) == -1) {
            std::cout << "Error setsockopt" << std::endl;
            return 1;
        }

        if (bind(listenfd, res->ai_addr, res->ai_addrlen) == 0)
            break;

        if (close(listenfd) < 0) {
            std::cout << "Error close" << std::endl;
            return 1;
        }
    } while ((res = res->ai_next) != NULL);

    if (res == NULL)
        err_sys("tcp_listen error");

    if (listen(listenfd, LISTENQ) < 0)
        err_sys("Error listen");
    if (addrlenp)
        *addrlenp = res->ai_addrlen;

    freeaddrinfo(ressave);

    return listenfd;
}

/*void str_cli(FILE *fp, int sockfd) {
    char sendline[MAXLINE], recvline[MAXLINE];
    while (fgets(sendline, MAXLINE, fp) != NULL) {
        if (writen(sockfd, sendline, strlen(sendline)) < 0) {
            std::cout << "Error writen" << std::endl;
            return;
        }
        if (readli)
    }
}*/
