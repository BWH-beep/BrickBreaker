#include <iostream>
#include <vector>
using namespace std;

// 抽象基类
class GameObject {
public:
    virtual void Update() = 0;
    virtual void Draw() = 0;
    virtual ~GameObject() {}
};

// Ball 类
class Ball : public GameObject {
private:
    int x, y;
public:
    Ball(int x, int y) : x(x), y(y) {
        cout << "构造 Ball 在 (" << x << ", " << y << ")" << endl;
    }
    ~Ball() { cout << "析构 Ball" << endl; }
    void Update() override { cout << "Ball 更新位置" << endl; }
    void Draw() override { cout << "Ball 绘制球体" << endl; }
};

// Paddle 类
class Paddle : public GameObject {
private:
    int x, width;
public:
    Paddle(int x, int w) : x(x), width(w) {
        cout << "构造 Paddle 在 x=" << x << ", 宽度=" << width << endl;
    }
    ~Paddle() { cout << "析构 Paddle" << endl; }
    void Update() override { cout << "Paddle 更新位置（响应键盘）" << endl; }
    void Draw() override { cout << "Paddle 绘制挡板" << endl; }
};

// Brick 类
class Brick : public GameObject {
private:
    int x, y, hp;
public:
    Brick(int x, int y, int hp) : x(x), y(y), hp(hp) {
        cout << "构造 Brick 在 (" << x << ", " << y << "), HP=" << hp << endl;
    }
    ~Brick() { cout << "析构 Brick" << endl; }
    void Update() override { cout << "Brick 检查碰撞状态" << endl; }
    void Draw() override { cout << "Brick 绘制砖块" << endl; }
};

int main() {
    vector<GameObject*> objects;
    
    // 动态创建对象
    objects.push_back(new Ball(400, 300));
    objects.push_back(new Paddle(350, 100));
    objects.push_back(new Brick(200, 100, 1));
    
    cout << "\n===== 游戏一帧：更新 =====\n";
    for (auto obj : objects) {
        obj->Update();
    }
    
    cout << "\n===== 游戏一帧：绘制 =====\n";
    for (auto obj : objects) {
        obj->Draw();
    }
    
    cout << "\n===== 释放所有对象 =====\n";
    for (auto obj : objects) {
        delete obj;
    }
    
    return 0;
}