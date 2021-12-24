#ifndef __SOCKET_HPP
#define __SOCKET_HPP

#include "header.hpp"
#include <exception>
#include <netdb.h>
#include <sys/socket.h>
#include <strings.h>
#include <unistd.h>
#include <string>

#define CLIENTS 32

class Socket {
private:
    int _sock_fd;
    struct addrinfo _hints;
    // Socket parameters;
    int _lvl;
    int _option;
    // int sock_family;
    // int socktype;

public:
    Socket();
    Socket(int sock_family, int socktype);
    Socket(int sock_fd);
    Socket(const Socket &);
    Socket& operator=(const Socket &);
    bool    operator<(const Socket& other);


    int get_fd() const; // return fd
    void Bind(const std::string& hostname, const std::string& port);
    // maybe add setsockopt
    void Listen(size_t backlog);
    Socket Accept() const;
    void Send(const std::string & to_send) const;
    std::string Recv() const;
    void Close();
    void Setsockopt(int lvl = SOL_SOCKET, int option = SO_REUSEADDR);
    ~Socket();
    class SocketError : public std::exception {
    private:
        std::string _err_msg;
    public:
        SocketError(const std::string& method, const std::string& error);
        virtual const char* what () const throw ();
        virtual ~SocketError() throw();
    };
};

bool operator<(const Socket& lhs, const Socket& rhs);
bool operator==(const Socket& lhs, const Socket& rhs);

#endif /* ifndef __SOCKET_HPP */
