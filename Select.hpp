#ifndef __SELECT_HPP
#define __SELECT_HPP

#include <sys/select.h>
#include "Socket.hpp"
#include <set>

enum Ready {
    READ,
    WRITE
};

typedef std::set<Socket> set_t;
// struct Sets {
//     set_t  readables;
//     set_t  writeables;
//     set_t  exeptables;
// };

class Select {
private:
    int     _maxfd;
    // fd_set  _set[3];
    // fd_set  _read;
    // fd_set  _write;
    // fd_set  _except; // Нужно ли?
    fd_set  _tmpread;
    fd_set  _tmpwrite;
    size_t  read_size;
    size_t  write_size;
    int     fd;

    Select(const Select& other);
    Select& operator=(const Select& other);

public:
    Select();
    ~Select();

    void Remove(const Socket& sock, int set);
    void Append(int set_type, const Socket& socket);
    void Clear(const Socket& sock, int set_type);
    std::pair<set_t, set_t> Selector(const set_t& read, const set_t& write);
    // bool Ready(const Socket& sock, int set_type);
};

#endif /* ifndef __SELECT_HPP */
