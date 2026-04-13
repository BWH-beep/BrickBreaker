#ifndef PADDLE_H
#define PADDLE_H

#include "raylib.h"

class Paddle {
private:
    Vector2 position;   // 板子的中心位置 (x, y)
    Vector2 size;       // 板子的尺寸 (宽, 高)
    float speed;        // 板子的移动速度（像素/秒）
    int screenWidth;    // 屏幕宽度（用于边界限制）
    int screenHeight;   // 屏幕高度（用于初始位置）

public:
    Paddle(int width, int height);  // 构造函数
    void Update(float dt);          // 根据左右键移动
    void Draw();                    // 绘制板子
    void Reset();                   // 重置到初始位置
    void SetWidth(float newWidth);
    
    Vector2 GetPosition();          // 获取位置
    Vector2 GetSize();              // 获取尺寸
    Rectangle GetRect();            // 获取碰撞矩形（用于碰撞检测）
};

#endif