#include <iostream>
#include <thread>
#include <chrono>
using namespace std;

// 模拟耗时任务
void loadResource(int id) {
    cout << "线程 " << id << " 开始加载资源..." << endl;
    this_thread::sleep_for(chrono::seconds(2));  // 模拟耗时2秒
    cout << "线程 " << id << " 加载完成!" << endl;
}

int main() {
    cout << "主线程：创建两个工作线程" << endl;
    
    // 创建线程
    thread t1(loadResource, 1);
    thread t2(loadResource, 2);
    
    cout << "主线程：等待工作线程完成..." << endl;
    
    // 等待线程结束
    t1.join();
    t2.join();
    
    cout << "主线程：所有线程完成，程序退出" << endl;
    return 0;
}
