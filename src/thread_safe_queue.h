#ifndef THREAD_SAFE_QUEUE_H
#define THREAD_SAFE_QUEUE_H

#include <queue>
#include <mutex>
#include <string>

class ThreadSafeQueue {
private:
    std::queue<std::string> q;
    mutable std::mutex mtx;

public:
    void push(std::string value) {
        std::lock_guard<std::mutex> lock(mtx);
        q.push(value);
    }

    bool pop(std::string& out) {
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

    int size() const {
        std::lock_guard<std::mutex> lock(mtx);
        return q.size();
    }
};

#endif
