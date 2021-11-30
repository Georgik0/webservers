//
// Created by Stephane Kitsch on 11/29/21.
//

#include "workers.hpp"

//#include <unistd.h>
#include <iostream>
#include <unistd.h>

int Worker::_count_worker = 0;
int *Worker::iget = 0;
int *Worker::clifd = 0;
int *Worker::iput = 0;
std::queue<int> *Worker::_clients = NULL;
pthread_mutex_t  *Worker::_clifd_mutex = NULL;
pthread_cond_t   *Worker::_clifd_cond = NULL;

Worker::Worker() {
    _index = 0;
//    _thread_tid = NULL;
//    _thread_tid = new pthread_t;
//    _thread_tid = (pthread_t *)malloc(sizeof(pthread_t));
//    if (_thread_tid == NULL)
//        write(2, "test1\n", strlen("test1\n"));
//    _clifd_mutex = 0;
//    _clifd_cond = 0;
}

Worker::~Worker() {}

/*Worker::Worker(const Worker &worker) {}*/

/*Worker &Worker::operator=(const Worker &worker) {}*/

int Worker::getIndex() {
    return _index;
}

void Worker::setCond(pthread_cond_t *clifd_cond) {
    _clifd_cond = clifd_cond;
}

void Worker::setMutex(pthread_mutex_t *clifd_mutex) {
    _clifd_mutex = clifd_mutex;
}

void Worker::setClients(std::queue<int> *clients) {
    _clients = clients;
}

pthread_cond_t *Worker::getCond() {
    return _clifd_cond;
}

pthread_mutex_t *Worker::getMutex() {
    return _clifd_mutex;
}

std::queue<int> *Worker::getClients() {
    return _clients;
}

static void web_child(int sockfd) {
//    int ntowrite;
    ssize_t nread;
    char buf[MAXLINE];
//    char line[MAXLINE], result[MAXN];

    for (;;) {
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

void *thread_main(void *arg) {
    int connfd;
    void web_child(int);

    Worker *worker = static_cast<Worker *>(arg);
    pthread_mutex_t *clifd_mutex = worker->getMutex();
    pthread_cond_t *clifd_cond = worker->getCond();
    std::queue<int> *clients = worker->getClients();

    std::cout << "Thread [" + std::to_string(worker->getIndex()) + "] starting\n";
    for (;;) {
        pthread_mutex_lock(clifd_mutex);  /* check error pthread_mutex_lock */

        /*while (*(worker->iget) == *(worker->iput)) {
            pthread_cond_wait(clifd_cond, clifd_mutex);  *//* check error pthread_cond_wait *//*
        }*/
        pthread_cond_wait(clifd_cond, clifd_mutex);  /* check error pthread_cond_wait */
        std::cout << "after pthread_cond_wait\n";

//        connfd = worker->clifd[*(worker->iget)];  /* Присоединенный сокет, который требуется обслужить */
        connfd = clients->front(); /* достали первого клиента из очереди */
        std::cout << "after connfd = clifd[iget]\n";

        /*if (++(*(worker->iget)) == MAXNCLI)
            *(worker->iget) = 0;*/
        pthread_mutex_unlock(clifd_mutex);
//        std::cout << *static_cast<int *>(arg) << "\n";
//        tptr[*static_cast<int *>(arg)].thread_count++;

        web_child(connfd);   /* обработка запроса */
        close(connfd);   /* check error close */
    }
    return arg;
}

//pthread_t   thread_tid;

void Worker::start() {
//    void    *thread_main(void *);
//    int err = 0;
    _count_worker++;
    _index = _count_worker;
    std::cout << "$" << _index << "$    ";
    Worker  *worker = this;
//    write(2, "test\n", strlen("test\n"));
    pthread_create(&_thread_tid, NULL, &thread_main, static_cast<void *>(worker));
//    if ( (err = pthread_create(&_thread_tid, NULL, &thread_main, worker)) < 0)
//        return "Error pthread_create";
//    return NULL;
}
