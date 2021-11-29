//
// Created by Stephane Kitsch on 10/24/21.
//

#ifndef WEBSERVERS_PTHREAD_WORKERS_HPP
#define WEBSERVERS_PTHREAD_WORKERS_HPP

#include <pthread.h>

typedef struct {
    pthread_t   thread_tid; /* Идентификатор потока */
    long        thread_count; /* Кол-во обработанных запросов */
} Thread;

Thread *tptr; /* Массив структур */

#define MAXNCLI 32
int clifd[MAXNCLI], iget, iput; /* clifd[MAXNCLI] - массив, в который главный поток записывает дескрипторы присоединенных сокетов
                                * iput - индекс в данном массиве для очередного элемента, записываемого в него главным потоком
                                *  */
pthread_mutex_t clifd_mutex;
pthread_cond_t clifd_cond;

//static int nthreads;
//pthread_mutex_t clifd_mutex = PTHREAD_MUTEX_INITIALIZER;
//pthread_cond_t clifd_cond = PTHREAD_COND_INITIALIZER;

#endif //WEBSERVERS_PTHREAD_WORKERS_HPP
