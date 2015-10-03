#ifndef MUTEX_H
#define MUTEX_H
#include <pthread.h>


class Mutex
{
    pthread_mutex_t mtx;

    public:
        Mutex() {
        pthread_mutex_init(&mtx, NULL);
    }

    int lock() {
        return  pthread_mutex_lock(&mtx);
    }

    int trylock() {
        return  pthread_mutex_lock(&mtx);
    }

    int unlock() {
        return  pthread_mutex_unlock(&mtx);
    }

    virtual ~Mutex() {
        pthread_mutex_destroy(&mtx);
    }

};

#endif // MUTEX_H
