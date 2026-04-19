#include <iostream>
using namespace std;

// 基类 Base1
class Base1 {
private:
    int a;
public:
    Base1(int a_val) : a(a_val) {
        cout << "构造 Base1(a=" << a << ")" << endl;
    }
    ~Base1() {
        cout << "析构 Base1" << endl;
    }
};

// 基类 Base2
class Base2 {
private:
    int b;
public:
    Base2(int b_val) : b(b_val) {
        cout << "构造 Base2(b=" << b << ")" << endl;
    }
    ~Base2() {
        cout << "析构 Base2" << endl;
    }
};

// 派生类 Derived，多重继承 Base1 和 Base2
class Derived : public Base1, public Base2 {
private:
    int c;
public:
    Derived(int a_val, int b_val, int c_val) 
        : Base1(a_val), Base2(b_val), c(c_val) {
        cout << "构造 Derived(c=" << c << ")" << endl;
    }
    ~Derived() {
        cout << "析构 Derived" << endl;
    }
};

int main() {
    cout << "===== 创建 Derived 对象 =====" << endl;
    Derived d(10, 20, 30);
    cout << "===== 程序结束，开始析构 =====" << endl;
    return 0;
}