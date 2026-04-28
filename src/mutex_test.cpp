#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>
using namespace std;

int counter = 0;         // 共享数据
mutex mtx;               // 互斥锁

// 不用锁——会出错
void unsafeAdd() {
    for (int i = 0; i < 100000; i++) {
        counter++;       // 数据竞争！
    }
}

// 用锁——安全
void safeAdd() {
    for (int i = 0; i < 100000; i++) {
        mtx.lock();
        counter++;
        mtx.unlock();
    }
}

int main() {
    // 测试不用锁
    counter = 0;
    thread t1(unsafeAdd);
    thread t2(unsafeAdd);
    t1.join();
    t2.join();
    cout << "不用锁的结果: " << counter << " (期望200000)" << endl;
    
    // 测试用锁
    counter = 0;
    thread t3(safeAdd);
    thread t4(safeAdd);
    t3.join();
    t4.join();
    cout << "用锁的结果:   " << counter << " (期望200000)" << endl;
    
    return 0;
}
