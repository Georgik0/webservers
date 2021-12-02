//
// Created by Stephane Kitsch on 11/29/21.
//

#include "workers.hpp"

#include <iostream>
#include <unistd.h>
#include <cstring>

int Worker::_count_worker = 0;
int Worker::_count_clients = 0;
int *Worker::iget = 0;
int *Worker::clifd = 0;
int *Worker::iput = 0;
fd_set *Worker::_allset = NULL;
std::queue<int> *Worker::_clients_queue = NULL;
pthread_mutex_t  *Worker::_clifd_mutex = NULL;
pthread_cond_t   *Worker::_clifd_cond = NULL;

Worker::Worker() {
    _index = 0;
    _connfd = -1;
    data_answer = NULL;

}

Worker::~Worker() {}

/*Worker::Worker(const Worker &worker) {}*/

/*Worker &Worker::operator=(const Worker &worker) {}*/

/* setters */
void Worker::setCond(pthread_cond_t *clifd_cond) {
    _clifd_cond = clifd_cond;
}

void Worker::setMutex(pthread_mutex_t *clifd_mutex) {
    _clifd_mutex = clifd_mutex;
}

void Worker::setClientsQueue(std::queue<int> *clients) {
    _clients_queue = clients;
}

void Worker::setConnfd(int connfd) {
    _connfd = connfd;
}

void Worker::setAllset(fd_set *allset) {
    _allset = allset;
}

void Worker::setReadFlag(bool rd) {
    _ready_read = rd;
}

void Worker::setWriteFlag(bool wr) {
    _ready_write = wr;
}


/* getters */
int Worker::getIndex() {
    return _index;
}

pthread_cond_t *Worker::getCond() {
    return _clifd_cond;
}

pthread_mutex_t *Worker::getMutex() {
    return _clifd_mutex;
}

std::queue<int> *Worker::getClientsQueue() {
    return _clients_queue;
}

fd_set *Worker::getAllset() {
    return _allset;
}

int Worker::getConnfd() {
    return _connfd;
}

bool Worker::getReadFlag() {
    return _ready_read;
}

bool Worker::getWriteFlag() {
    return _ready_write;
}

int Worker::getCountClients() {
    return _count_clients;
}

void Worker::incrementCountClients() {
    _count_clients++;
}

void Worker::decrementCountClients() {
    _count_clients--;
}

static void web_child(Worker *worker) {
    ssize_t nread;
    char buf[MAXLINE];
    int sockfd = worker->getConnfd();
    fd_set *allset = worker->getAllset();

    for (;;) {
        if (worker->getReadFlag()) {
            if ( (nread = read(sockfd, buf, MAXLINE)) <= 0) {
//                close(sockfd); /* соединение закрыто клиентом */
                FD_CLR(sockfd, allset);
                worker->setConnfd(-1);
                worker->setReadFlag(false);
                return;
            } else {
                std::cout << "nread = " << nread << std::endl;
                buf[nread] = '\0';
                std::cout << "[" << sockfd << "] " << "Прочитал: " << buf << std::endl;
                worker->data_answer = new std::string("Data_answer");
                std::memset(buf, '\0', nread);
            }
            worker->setReadFlag(false);
        }

        if (worker->data_answer != NULL && worker->getWriteFlag()) {
//            std::string answer(buf);
//            answer = "\"" + answer + "\"" + " - server response";
            std::string answer(*worker->data_answer);
            if (writen(sockfd, answer.c_str(), answer.length()) < 0){
                std::cout << "sockfd = " << sockfd << "\n";
                err_sys("Error write");
            } else {
                std::cout << "Ответил: " << answer << std::endl;
            }
            delete worker->data_answer;
            worker->data_answer = NULL;
            worker->setWriteFlag(false);
        }
    }
}

void *thread_main(void *arg) {
    int connfd;
    void web_child(Worker *);

    Worker *worker = static_cast<Worker *>(arg);
    pthread_mutex_t *clifd_mutex = worker->getMutex();
    pthread_cond_t *clifd_cond = worker->getCond();
    std::queue<int> *clients_queue = worker->getClientsQueue();

    std::cout << "Thread [" + std::to_string(worker->getIndex()) + "] starting\n";
    for (;;) {
        pthread_mutex_lock(clifd_mutex);  /* check error pthread_mutex_lock */

        pthread_cond_wait(clifd_cond, clifd_mutex);  /* check error pthread_cond_wait */
        std::cout << "after pthread_cond_wait\n";

        connfd = clients_queue->front(); /* достали первого клиента из очереди */
        worker->setConnfd(connfd); /* закрепили за воркером клиента */
        clients_queue->pop(); /* удалили клиента из очереди */

        pthread_mutex_unlock(clifd_mutex);

        worker->incrementCountClients();
        web_child(worker);   /* обработка запроса */
        worker->decrementCountClients();

        close(connfd);   /* check error close */
    }
    return arg;
}

void Worker::start() {
    _count_worker++;
    _index = _count_worker;
    Worker  *worker = this;
    pthread_create(&_thread_tid, NULL, &thread_main, static_cast<void *>(worker));
}
