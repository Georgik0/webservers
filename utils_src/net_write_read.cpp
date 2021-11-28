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

void err_exit(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

int ft_socket(int family, int sock_type, int protocol) {
    int sockfd;

    sockfd = socket(family, sock_type, protocol);
    if (sockfd < 0)
        err_exit("[Socket]");
    return sockfd;
}

void ft_connect(int fd, const struct sockaddr* sa, socklen_t addrlen) {
    if (connect(fd, sa, addrlen))
        err_exit("[Connect]");
}

void    ft_bind(int fd, const struct sockaddr* addr, socklen_t addrlen) {
    if (bind(fd, addr, addrlen) < 0)
        err_exit("[Bind]");
}

void    ft_listen(int fd, int queue) {
    if (listen(fd, queue))
        err_exit("[Listen]");
}

int ft_accept(int listen_fd, struct sockaddr* sa, socklen_t* addrlen) {
    int connect_fd;

    connect_fd = accept(listen_fd, sa, addrlen);
    if (connect_fd < 0)
        err_exit("[Accept]");
    return connect_fd;
}

void sig_int(int n) {
    std::cout << "Хз что тут должно быть\n";
    std::cout << "n = " << n << std::endl;
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
