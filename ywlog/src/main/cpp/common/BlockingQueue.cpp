//
// Created by 杨浩 on 2016/10/24.
//
#include "BlockingQueue.h"
#include "../MMapLogger.h"
#include "Log.h"

template<typename T>
void BlockingQueue<T>::push(const T &item) {
    pthread_mutex_lock(&mutex);

    queue.push(item);
    pthread_cond_signal(&notEmpty);

    pthread_mutex_unlock(&mutex);;
}

template<typename T>
T BlockingQueue<T>::poll() {
    pthread_mutex_lock(&mutex);

    while(queue.empty()){
        pthread_cond_wait(&notEmpty,&mutex);
    }

    T front(queue.front());
    queue.pop();

    pthread_mutex_unlock(&mutex);
    return front;
}

template<typename T>
size_t BlockingQueue<T>::size() {
    pthread_mutex_lock(&mutex);

    size_t size = queue.size();

    pthread_mutex_unlock(&mutex);
    return size;
}

template<typename T>
bool BlockingQueue<T>::empty() {
    pthread_mutex_lock(&mutex);

    bool empty = queue.empty();

    pthread_mutex_unlock(&mutex);
    return empty;
}


template
class BlockingQueue<int>;

template
class BlockingQueue<float>;

template
class BlockingQueue<double>;

template
class BlockingQueue<LogInfo*>;