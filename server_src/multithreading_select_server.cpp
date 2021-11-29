//
// Created by Stephane Kitsch on 11/26/21.
//

#include <pthread.h>
#include <sys/socket.h> // socklen_t, struct sockaddr
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>

#include <iostream>

//#include "../utils_src/net_write_read.hpp"
//#include "pthread_workers.hpp"
#include "workers.hpp"

#define MAXNCLI 32
int clifd[MAXNCLI], iget, iput; /* clifd[MAXNCLI] - массив, в который главный поток записывает дескрипторы присоединенных сокетов
                                * iput - индекс в данном массиве для очередного элемента, записываемого в него главным потоком
                                *  */

static int nthreads;
pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

/* стр. 889 */ /* ./server 127.0.0.1 8000 */
int main(int argc, char **argv) {
    int     i, listenfd, connfd;
    void    sig_int(int), thread_make(int);
    socklen_t addrlen = 0, clilen;
    struct sockaddr *cliaddr;
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
    /*tptr = (Thread *)calloc(nthreads, sizeof(Thread));
    if (tptr == NULL)
        std::cout << "Err calloc\n";*/

    workers = new Worker[nthreads];
    workers[0].setMutex(&clifd_mutex);
    workers[0].setCond(&clifd_cond);
    workers[0].iput = &iput;
    workers[0].iget = &iget;
    workers[0].clifd = clifd;

    std::cout << "TEST\n";
    /* создание всех потоков */
//    for (i = 0; i < nthreads; i++)
//        thread_make(i);
    for (i = 0; i < nthreads; i++)
        workers[i].start();

//    signal(SIGINT, sig_int);

    for (;;) {
        clilen = addrlen;
        connfd = ft_accept(listenfd, cliaddr, &clilen);
        write(1, "Server get accept\n", strlen("Server get accept\n"));

        pthread_mutex_lock(&clifd_mutex); /* check error pthread_mutex_lock */
        clifd[iput] = connfd;
        if (++iput == MAXNCLI)
            iput = 0;
        if (iput == iget)
            err_sys("iput = iget");
        pthread_cond_signal(&clifd_cond); /* check error pthread_cond_signal */
        pthread_mutex_unlock(&clifd_mutex); /* check error pthread_mutex_unlock */
    }
}

/*
void thread_make(int i) {
    void    *thread_main(void *);

    pthread_create(&tptr[i].thread_tid, NULL, &thread_main, &i); */
/* check error pthread_create *//*

    return;
}

void *thread_main(void *arg) {
    int connfd;
    void web_child(int);

    std::cout << "Thread starting\n";
    for (;;) {
        pthread_mutex_lock(&clifd_mutex); */
/* check error pthread_mutex_lock *//*

        while (iget == iput) {
            pthread_cond_wait(&clifd_cond, &clifd_mutex); */
/* check error pthread_cond_wait *//*

        }
        std::cout << "after pthread_cond_wait\n";
        connfd = clifd[iget]; */
/* Присоединенный сокет, который требуется обслужить *//*

        std::cout << "after connfd = clifd[iget]\n";

        if (++iget == MAXNCLI)
            iget = 0;
        pthread_mutex_unlock(&clifd_mutex);
//        std::cout << *static_cast<int *>(arg) << "\n";
        tptr[*static_cast<int *>(arg)].thread_count++;

        web_child(connfd);  */
/* обработка запроса *//*

        close(connfd);  */
/* check error close *//*

    }
    return NULL;
}

void web_child(int sockfd) {
//    int ntowrite;
    ssize_t nread;
    char buf[MAXLINE];
//    char line[MAXLINE], result[MAXN];

    for (;;) {
*/
/*        if ( (nread = read(sockfd, line, MAXLINE)) == 0 )
            return;     *//*
*/
/* соединение закрыто другим концом *//*
*/
/*

        *//*
*/
/* line задает кол-во байт, которое следует отправить обратно *//*
*/
/*
        ntowrite = atol(line);
        if ( (ntowrite <= 0) || (ntowrite > MAXN) )
            err_exit("client request err ntowrite");
        if ( writen(sockfd, result, ntowrite) < 0)
            err_exit("error writen");*//*


        if ( (nread = read(sockfd, buf, MAXLINE)) < 0) {
            std::cout << "Прочитал: " << buf << std::endl;
            return;
        } else {
            std::cout << "[" << sockfd << "] " << "Прочитал: " << buf << std::endl;
        }

        std::string answer(buf);
        answer = "\"" + answer + "\"" + " - server response";
        if (writen(sockfd, answer.c_str(), answer.length()) < 0){
            err_sys("Error write");
        } else {
            std::cout << "Ответил: " << answer << std::endl;
        }

    }
}
*/
