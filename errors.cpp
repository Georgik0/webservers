#include "header.hpp"
#include <cstdlib>

void err_exit(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}
