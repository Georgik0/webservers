//
// Created by Stephane Kitsch on 10/24/21.
//

#include <stdio.h>
#include <fcntl.h>
#include <sys/select.h>
#include <unistd.h>
#include <iostream>

#define	MAXLINE		4096	/* max text line length */

void err_sys(const char* x) {
    perror(x);
    exit(1);
}

int Fcntl(int fd, int cmd, int arg) {
    int val;
    std::string err;

    switch (cmd) {
        case F_GETFL:
            err = "F_GETFL error";
            break;
        case F_SETFL:
            err = "F_SETFL error";
            break;
        default:
            break;
    }
    if (val == fcntl(fd, cmd, arg) < 0)
        err_sys(err.c_str());
    return val;
}

void str_cli(FILE *fp, int sockfd) {
    int     maxfdp1, val, stdineof;
    ssize_t n, nwritten;
    fd_set  rset, wset;
    char    to[MAXLINE], fr[MAXLINE];
    char    *toiptr, *tooptr, *friptr, *froptr;

    val = Fcntl(sockfd, F_GETFL, 0);
    Fcntl(sockfd, F_SETFL, val | O_NONBLOCK);

    val = Fcntl(STDOUT_FILENO, F_GETFL, 0);
    Fcntl(STDOUT_FILENO, F_SETFL, val | O_NONBLOCK);

    val = Fcntl(STDIN_FILENO, F_GETFL, 0);
    Fcntl(STDIN_FILENO, F_SETFL, val | O_NONBLOCK);

    toiptr = tooptr = to; /* Инициализация указателей буфера */
    friptr = froptr = fr;
    stdineof = 0;

    maxfdp1 = std::max(std::max(STDIN_FILENO, STDOUT_FILENO), sockfd) + 1;
    for (;;) {
        FD_ZERO(&rset);
        FD_ZERO(&wset);
        if (stdineof == 0 && toiptr < &to[MAXLINE])
            FD_SET(STDIN_FILENO, &rset); /* чтение из стандартного потока ввода */
        if (friptr < &fr[MAXLINE])
            FD_SET(sockfd, &rset); /* чтение из сокета */
        if (tooptr != toiptr)
            FD_SET(sockfd, &wset); /* данные для записи в сокет */
        if (froptr != friptr)
            FD_SET(STDOUT_FILENO, &wset); /* данные для записи в стандартный поток вывода */
        select(maxfdp1, &rset, &wset, NULL, NULL);
    }
}
