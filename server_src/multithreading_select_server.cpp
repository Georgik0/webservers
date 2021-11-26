//
// Created by Stephane Kitsch on 11/26/21.
//

#include <pthread.h>

static int nthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

/* стр. 889 */
int main(int argc, char **argv) {
    int     i, listenfd, connfd;
    void    sig_int
}



/*
 * 1
 * 2--
 * 3--
 * 4--
 *
 * */