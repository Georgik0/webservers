#include "header.hpp"
#include <iostream>
//#include <sys/_types/_pid_t.h>
#include <sys/wait.h>
#include <sstream>

void    sig_child(int signo) {
    pid_t   pid;
    int     stat;

    while ((pid = waitpid(-1, &stat, WNOHANG)) > 0)
        std::cout << "child " << pid << " finished\n";
}

namespace ft {
    std::string to_string(int n) {
        std::ostringstream ss;
        ss << n;
        return ss.str();
    }
}
