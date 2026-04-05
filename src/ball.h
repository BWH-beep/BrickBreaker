#ifndef BALL_H
#define BALL_H

#include "raylib.h"

class Ball {
private:
    Vector2 position;   // 球的位置 (x, y)
    Vector2 speed;      // 球的速度 (vx, vy)
    float radius;       // 球的半径
    bool moving;        // 球是否在移动中

public:
    Ball();                             // 构造函数
    void Update(float dt);              // 更新位置
    void Draw();                        // 绘制球
    void Start();                       // 开始移动
    void Stop();                        // 停止移动
    void Reset();                       // 重置到初始位置
    void BounceX();                     // X方向反弹
    void BounceY();                     // Y方向反弹
    void SetPositionX(float x);         // 设置X坐标
    void SetPositionY(float y);         // 设置Y坐标
    void AddSpeedX(float delta);        // 增加X速度
    void ClampSpeed(float maxSpeed);    // 限制最大速度
    
    Vector2 GetPosition();              // 获取位置
    Vector2 GetSpeed();                 // 获取速度
    float GetRadius();                  // 获取半径
    bool IsMoving();                    // 是否在移动
    bool IsOutOfScreen(int screenHeight); // 是否掉出屏幕
};

#endif