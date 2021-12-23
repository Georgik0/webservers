//
// Created by Stephane Kitsch on 11/29/21.
//

#ifndef WEBSERVERS_WORKERS_HPP
#define WEBSERVERS_WORKERS_HPP

#include <string>
#include <queue>

#include <pthread.h>
#include "./utils_src/net_write_read.hpp"

#define MAXNCLI 32

class Worker {
private:
    int                     _index; /* Номер потока */
    pthread_t               _thread_tid; /* Идентификатор потока */
    int                     _connfd;
    bool                    _ready_write;
    bool                    _ready_read;
//    long        _thread_count; /* Кол-во обработанных запросов */
    /*static void *_thread_main(void *arg);*/
    static int              _count_worker; /* Кол-во воркеров */
    static int              _count_clients; /* Кол-во обрабатываемых клиентов в данный момент */
    static pthread_mutex_t  *_clifd_mutex;
    static pthread_cond_t   *_clifd_cond;
    static std::queue<int>  *_clients_queue;
    static fd_set           *_allset;
public:
    Worker();
    ~Worker();
    /*Worker(Worker const &worker);
    Worker &operator=(Worker const &worker);*/
    void start();
    void incrementCountClients();
    void decrementCountClients();

    /* getters */
    int getIndex();
    fd_set *getAllset();
    pthread_mutex_t *getMutex();
    pthread_cond_t *getCond();
    std::queue<int> *getClientsQueue();
    int getConnfd();
    bool getWriteFlag();
    bool getReadFlag();
    static int getCountClients();

    /* setters */
    void setWriteFlag(bool wr);
    void setReadFlag(bool rd);
    void setConnfd(int connfd);
    static void setAllset(fd_set *allset);
    static void setMutex(pthread_mutex_t *clifd_mutex);
    static void setCond(pthread_cond_t *clifd_cond);
    static void setClientsQueue(std::queue<int> *clients);

    std::string   data_answer;

    static int              *clifd;
    static int              *iget;
    static int              *iput;
};

#endif //WEBSERVERS_WORKERS_HPP
