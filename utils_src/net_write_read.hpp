//
// Created by Stephane Kitsch on 10/21/21.
//

#ifndef NETWORK_PROGRAMMING_NET_WRITE_READ_HPP
#define NETWORK_PROGRAMMING_NET_WRITE_READ_HPP

#define MAXLINE 4096
#define	LISTENQ	1024 /* максимальное кол-во клиентских соединений */
#define MAXN 16384 /* максимальное количество байтов, которое клиент может запросить */
#define	SA	struct sockaddr

#include <sys/types.h> /* ssize_t fd_set */
#include <sys/socket.h> /* socklen_t */
#include <sys/time.h>

ssize_t writen(int fd, const void *vptr, size_t n);
int Tcp_listen(const char *host, const char *serv, socklen_t *addrlenp);

//  Wrappers
int ft_socket(int family, int sock_type, int protocol);
void ft_connect(int fd, const struct sockaddr* sa, socklen_t addrlen);
void ft_bind(int fd, const struct sockaddr* addr, socklen_t addrlen);
void ft_listen(int fd, int queue);
int ft_accept(int listen_fd, struct sockaddr* sa, socklen_t* addrlen);
void ft_select(int maxfdp1, fd_set *readset, fd_set *writeset, fd_set *exceptset, struct timeval *timeout);

//  Exit
void err_exit(const char* msg);
void err_sys(const char* x);

#endif //NETWORK_PROGRAMMING_NET_WRITE_READ_HPP
