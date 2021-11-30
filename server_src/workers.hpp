//
// Created by Stephane Kitsch on 11/29/21.
//

#ifndef WEBSERVERS_WORKERS_HPP
#define WEBSERVERS_WORKERS_HPP

#include <string>
#include <queue>

#include <pthread.h>
#include "../utils_src/net_write_read.hpp"

#define MAXNCLI 32

class Worker {
private:
    int                     _index; /* Номер потока */
    pthread_t               _thread_tid; /* Идентификатор потока */
//    long        _thread_count; /* Кол-во обработанных запросов */
    /*static void *_thread_main(void *arg);*/
    static int              _count_worker; /* Кол-во воркеров */
    static pthread_mutex_t  *_clifd_mutex;
    static pthread_cond_t   *_clifd_cond;
    static std::queue<int>  *_clients;
public:
    Worker();
    ~Worker();
    /*Worker(Worker const &worker);
    Worker &operator=(Worker const &worker);*/
    void start();
    int getIndex();
    static void setMutex(pthread_mutex_t *clifd_mutex);
    static void setCond(pthread_cond_t *clifd_cond);
    static void setClients(std::queue<int> *clients);
    pthread_mutex_t *getMutex();
    pthread_cond_t *getCond();
    std::queue<int> *getClients();
    /*static void    *thread_main(void *arg);*/
    static int              *clifd;
    static int              *iget;
    static int              *iput;
};

#endif //WEBSERVERS_WORKERS_HPP
