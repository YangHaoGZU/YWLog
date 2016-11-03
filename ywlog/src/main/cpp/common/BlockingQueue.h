//
// Created by 杨浩 on 2016/10/24.
//

#ifndef BLOCKINGQUEUE_H
#define BLOCKINGQUEUE_H

#include <queue>
#include <pthread.h>

template<typename T>
class BlockingQueue{
public:
    BlockingQueue(){
        pthread_mutex_init(&mutex, NULL);
        pthread_cond_init(&notEmpty, NULL);
    }
    ~BlockingQueue(){
        pthread_mutex_destroy(&mutex);
        pthread_cond_destroy(&notEmpty);
    }
    void push(const T& item);
    T poll();
    size_t size();
    bool empty();

private:
    pthread_mutex_t mutex;
    pthread_cond_t notEmpty;

    std::queue<T> queue;
};

#endif //BLOCKINGQUEUE_H
