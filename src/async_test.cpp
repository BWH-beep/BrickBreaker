#include <iostream>
#include <future>
#include <chrono>
#include <string>
#include <thread>
using namespace std;

// 模拟加载关卡数据
string loadLevel(int levelId) {
    cout << "后台线程：开始加载关卡 " << levelId << "..." << endl;
    this_thread::sleep_for(chrono::seconds(3));  // 模拟耗时3秒
    cout << "后台线程：关卡 " << levelId << " 加载完成!" << endl;
    return "关卡" + to_string(levelId) + "的数据";
}

int main() {
    cout << "主线程：发起异步加载..." << endl;
    
    // 异步执行，不阻塞主线程
    future<string> result = async(launch::async, loadLevel, 1);
    
    cout << "主线程：加载中，可以做其他事..." << endl;
    
    // 模拟主线程在忙（比如刷新画面）
    for (int i = 1; i <= 5; i++) {
        cout << "主线程：绘制第 " << i << " 帧" << endl;
        this_thread::sleep_for(chrono::milliseconds(500));
    }
    
    cout << "主线程：等待加载结果..." << endl;
    string data = result.get();  // 阻塞直到结果就绪
    cout << "主线程：收到数据: " << data << endl;
    
    return 0;
}
