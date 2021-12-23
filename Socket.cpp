#include "Socket.hpp"

#include <strings.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdio>
#include <cstring>
#include <iostream>
#include <string>
#include <type_traits>

#include "header.hpp"

Socket::Socket() {
    bzero(&_hints, sizeof(_hints));
    _hints.ai_family = AF_UNSPEC;
    _hints.ai_socktype = SOCK_STREAM;
}

Socket::Socket(int sock_family, int socktype) {
    bzero(&_hints, sizeof(_hints));
    _hints.ai_family = sock_family;
    _hints.ai_socktype = socktype;
}

Socket::Socket(const Socket& other)
    : _sock_fd(other._sock_fd), _hints(other._hints) {}

Socket::Socket(int sock_fd) : _sock_fd(sock_fd) {
    bzero(&_hints, sizeof(_hints));
}

Socket& Socket::operator=(const Socket& other) {
    if (this == &other) return (*this);
    _sock_fd = other._sock_fd;
    _hints = other._hints;
    return (*this);
}

bool Socket::operator<(const Socket& other) {
    return (_sock_fd < other._sock_fd);
}

int Socket::get_fd() const { return (_sock_fd); }

void Socket::Bind(const std::string& hostname, const std::string& port) {
    struct addrinfo *res, *ressave;
    int n;
    int on = 1;

    _hints.ai_flags = AI_PASSIVE;
    n = getaddrinfo(hostname.c_str(), port.c_str(), &_hints, &res);
    ressave = res;  // save data to free after binding
    for (; res != NULL; res = res->ai_next) {
        if ((_sock_fd = socket(res->ai_family, res->ai_socktype,
                               res->ai_protocol)) < 0)
            continue;
        setsockopt(_sock_fd, _lvl, _option, &on, sizeof(on));
        if (!bind(_sock_fd, res->ai_addr, res->ai_addrlen)) break;
        close(_sock_fd); /* bind error, close and try next one */
    }
    if (res == NULL) {
        throw SocketError("Bind: ", strerror(errno));
    }
    freeaddrinfo(ressave);
}

void Socket::Listen(size_t backlog) {
    if (listen(_sock_fd, backlog) < 0) {
        Socket::Close();
        throw SocketError("Listen: ", strerror(errno));
    }
}

Socket Socket::Accept() const {
    Socket test;
    if ((test._sock_fd = accept(_sock_fd, NULL, NULL)) < 0) {
        throw SocketError("Accept: ", strerror(errno));
    }
    return (test);
}

void Socket::Send(std::string& to_send) const {
    int n;
    n = write(_sock_fd, to_send.c_str(), to_send.length());
    if (n == 0) {
        throw SocketError("Send: ", "Closed connection with client " +
                                        ft::to_string(_sock_fd) + "\n");
    } else if (n < 0) {
        throw SocketError("Send: ", strerror(errno));
    }
}

std::string Socket::Recv() const {
    char recvdata[MAXLINE];
    int n;
    bzero(recvdata, sizeof(recvdata));
    n = read(_sock_fd, recvdata, sizeof(recvdata));
    if (n == 0) {
        throw SocketError("Recv: ", "Closed connection with client " +
                                        ft::to_string(_sock_fd) + "\n");
    } else if (n < 0) {
        throw SocketError("Recv: ", strerror(errno));
    }
    return recvdata;
}

void Socket::Close() { close(_sock_fd); }

void Socket::Setsockopt(int lvl, int option) {
    _lvl = lvl;
    _option = option;
}

Socket::~Socket() {}

bool operator<(const Socket& lhs, const Socket& rhs) {
    return (lhs.get_fd() < rhs.get_fd());
}

Socket::SocketError::SocketError(const std::string& method,
                                 const std::string& error)
    : _err_msg(method) {
    _err_msg.append(error);
}

const char* Socket::SocketError::what() const throw() {
    return _err_msg.c_str();
}

Socket::SocketError::~SocketError() throw() {}
