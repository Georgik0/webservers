#include "header.hpp"
#include <sys/socket.h>

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
