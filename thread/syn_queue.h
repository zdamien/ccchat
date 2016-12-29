// Standard synchronized queue class.

#ifndef CCCH_SQUEUE_H
#define CCCH_SQUEUE_H

#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>

template<class T>
class syn_queue {
    std::queue<T> q;
    std::mutex m;
    std::condition_variable cond;
public:
    void put(const T& value);
    void put(T&& value);
    void get(T& val);  
    // I'd prefer enqueue() and a returning get(), but I'm following
    // Stroustrup.
};

// oh right, template implementation can't be easily isolated in .cc
// files.  Grrr.

template<class T>
void syn_queue<T>::put(const T& value) {
    std::unique_lock<std::mutex> lck{m};
    q.push(value);
    cond.notify_one();
}

template<class T>
void syn_queue<T>::put(T&& value) {
    std::unique_lock<std::mutex> lck{m};
    q.push(value);
    cond.notify_one();
}


template<class T>
void syn_queue<T>::get(T& val) {
    std::unique_lock<std::mutex> lck{m};
    while (q.empty())
        cond.wait(lck);
    val = q.front();
    q.pop();
}

#endif
