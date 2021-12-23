#include "Select.hpp"
#include <algorithm>
#include <iostream>
//#include <sys/_types/_fd_def.h>
#include <unistd.h>
#include <utility>

Select::Select() : _maxfd(2), read_size(0), write_size(0), fd(0) {
    FD_ZERO(&_tmpread);
    FD_ZERO(&_tmpwrite);
}

Select::~Select() { }

std::pair<set_t, set_t> Select::Selector(const set_t& read, const set_t& write) {
    set_t   readable, writeable;
    fd_set  readset, writeset;
    set_t::iterator it;
    int nready, maxfd;

    if (read.size() > read_size) {
        for (it = read.begin(); it != read.end(); it++) {
            FD_SET(it->get_fd(), &_tmpread);
        }
        fd = read.rbegin()->get_fd();
    }
    if (write.size() > write_size) {
        for (it = write.begin(); it != write.end(); it++) {
            FD_SET(it->get_fd(), &_tmpwrite);
        }
    }
    _maxfd = fd;
    readset = _tmpread;
    writeset = _tmpwrite;
    // std::cout << "-> Before select <- maxfd = " << _maxfd << std::endl;
    nready = select(_maxfd + 1, &readset, &writeset, NULL, NULL);
    if (nready) {
        for (it = read.begin(); it != read.end(); it++) { 
            if (FD_ISSET(it->get_fd(), &readset)) {
                readable.insert(*it);
            }
        }
        for (it = write.begin(); it != write.end(); it++) { 
            if (FD_ISSET(it->get_fd(), &writeset)) {
                writeable.insert(*it);
            }
        }
    }
    read_size = read.size();
    write_size = write.size();
    return std::make_pair(readable, writeable);
}

void Select::Remove(const Socket& sock, int set) {
    if (set == READ) {
        FD_CLR(sock.get_fd(), &_tmpread);
    } else if (set == WRITE) {
        FD_CLR(sock.get_fd(), &_tmpwrite);
    }
}
