//
// Created by Stephane Kitsch on 11/26/21.
//

#include <pthread.h>
#include <sys/socket.h> // socklen_t, struct sockaddr
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <cstring>

#include <iostream>
#include <queue>

//#include "../utils_src/net_write_read.hpp"
//#include "pthread_workers.hpp"
#include "workers.hpp"

#define MAXNCLI 32

static int nthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

/* ./server 127.0.0.1 8000 */
int main(int argc, char **argv) {
    int     i, listenfd, connfd;
    void    sig_int(int), thread_make(int);
    socklen_t addrlen = 0, clilen;
    struct sockaddr *cliaddr;
    std::queue<int> clients;
    Worker *workers;

    if (argc == 3)
        listenfd = Tcp_listen(NULL, argv[1], &addrlen);
    else if (argc == 4)
        listenfd = Tcp_listen(argv[1], argv[2], &addrlen);
    else
        err_sys("err argv");

    if ( (cliaddr = (SA *)malloc(addrlen)) == NULL )
        err_sys("Error malloc");
    if ( (nthreads = atoi(argv[argc - 1])) <= 0)
        err_sys("Error nthreads");


    workers = new Worker[nthreads];
    Worker::setMutex(&clifd_mutex);
    Worker::setCond(&clifd_cond);
    Worker::setClientsQueue(&clients);

    std::cout << "TEST\n";
    for (i = 0; i < nthreads; i++)
        workers[i].start();

    fd_set  read_set, write_set, allset;
    int client_fd, maxfd = listenfd;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    Worker::setAllset(&allset);

    for (;;) {
        read_set = allset;
        write_set = allset;
//        FD_CLR(listenfd, write_set);
        if (select(maxfd + 1, &read_set, &write_set, NULL, NULL) < 0)
            err_sys("Error select\n");

        if (Worker::getCountClients() == nthreads) {
//            std::cout << "To many clients\n";
        } else {
            if (FD_ISSET(listenfd, &read_set)) {
                clilen = addrlen;
                connfd = ft_accept(listenfd, cliaddr, &clilen);
                write(1, "Server get accept\n\n", strlen("Server get accept\n\n"));

                pthread_mutex_lock(&clifd_mutex); /* check error pthread_mutex_lock */

                clients.push(connfd);

                pthread_cond_signal(&clifd_cond); /* check error pthread_cond_signal */
                pthread_mutex_unlock(&clifd_mutex); /* check error pthread_mutex_unlock */
                FD_SET(connfd, &allset);    /* добавление нового дескриптора */
                if (connfd > maxfd)
                    maxfd = connfd;     /* для ф-ции select */
            }
        }

        for (i = 0; i < nthreads; i++) {
            if ( (client_fd = workers[i].getConnfd()) == -1)
                continue;
            if (FD_ISSET(client_fd, &read_set)) {
                workers[i].setReadFlag(true);
            }
            if (FD_ISSET(client_fd, &write_set)) {
                workers[i].setWriteFlag(true);
            }
        }
    }
}
