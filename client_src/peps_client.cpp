//
// Created by Stephane Kitsch on 10/20/21.
//

//#include "header.hpp"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <istream>
#include <string>
#include <strings.h>
#include <sys/_select.h>
#include <sys/_types/_fd_def.h>
#include <sys/socket.h>
#include <unistd.h>

#define CLIENTS 1
void    str_client(int connect_fd) {
    char    recvline[MAXLINE];
    int stdineof = 0;
    fd_set  rset;
    int     maxdp;
    std::string    sendline;

    FD_ZERO(&rset);
    for ( ;; ) {
        if (stdineof == 0) // неизвестно зачем
            FD_SET(STDIN_FILENO, &rset);
        FD_SET(connect_fd, &rset);
        maxdp = std::max(STDIN_FILENO, connect_fd) + 1;
        select(maxdp, &rset, NULL, NULL, NULL);

        if (FD_ISSET(connect_fd, &rset)) {
            if (read(connect_fd, recvline, sizeof(recvline)) == 0) {
                if (stdineof == 1) {
                    return;
                } else {
                    err_exit("server ended connection");
                }
            }
            std::cout << "-> " << recvline << std::endl;
        }
        if (FD_ISSET(STDIN_FILENO, &rset)) {
            if (!std::getline(std::cin, sendline)) {
                stdineof = 1;
                shutdown(connect_fd, SHUT_WR);
                FD_CLR(STDIN_FILENO, &rset);  //    Неизвестно зачем
                continue;
            }
            write(connect_fd, sendline.c_str(), sendline.length());
        }
        bzero(recvline, sizeof(recvline));
    }
}

int main(int argc, char **argv) {
    int tcp_socket[CLIENTS];
    char    recvline[MAXLINE];
    struct sockaddr_in servaddr;

    if (argc != 2)
        err_exit("[Usage]: ip adress");

    bzero(&servaddr, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) < 0)
        err_exit("[inet_pton]: error");
    for (int i = 0; i < CLIENTS; i++) {
        tcp_socket[i] = ft_socket(AF_INET, SOCK_STREAM, 0);
        ft_connect(tcp_socket[i], reinterpret_cast<SA *>(&servaddr), sizeof(servaddr));
    }
    str_client(tcp_socket[0]);
    exit(EXIT_SUCCESS);
    return 0;
}

