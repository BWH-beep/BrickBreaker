#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>

template<typename T>
class ThreadSafeQueue {
private:
    std::queue<T> q;
    mutable std::mutex mtx;
    
public:
    void push(T value) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(value);
    }
    
    bool pop(T& out) {
        std::lock_guard<std::mutex> lock(mtx);
        if (q.empty()) return false;
        out = q.front();
        q.pop();
        return true;
    }
    
    bool empty() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.empty();
    }
};

#endif
