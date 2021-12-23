#ifndef __HEADER_HPP
#define __HEADER_HPP

#include <arpa/inet.h>
#include <map>
#include <netinet/ip.h>
//#include <sys/_endian.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <string>

#define MAXLINE 128000
#define PORT 9877
#define SA struct sockaddr

//  Wrappers
int ft_socket(int family, int sock_type, int protocol);
void ft_connect(int fd, const struct sockaddr* sa, socklen_t addrlen);
void ft_bind(int fd, const struct sockaddr* addr, socklen_t addrlen);
void ft_listen(int fd, int queue);
int ft_accept(int listen_fd, struct sockaddr* sa, socklen_t* addrlen);

// Utils
void sig_child(int signo);

//  Exit
void err_exit(const char* msg);

std::map<std::string, std::string> parse_header(const std::string & str, char* msg_end);

namespace ft {
    std::string to_string(int n);
}
// std::vector<std::vector<ConfigParser> > parse_config(str configname);

#endif /* ifndef __HEADER_HPP */
