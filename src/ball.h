#ifndef BALL_H
#define BALL_H

#include "raylib.h"
#include <vector>

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    float radius;
    bool moving;
    Vector2 originalSpeed;
    std::vector<Vector2> trail;  // 拖尾历史位置

public:
    Ball();
    void Update(float dt);
    void Draw();
    void Start(float speedX, float speedY);
    void Stop();
    void Reset();
    void BounceX();
    void BounceY();
    void SetPositionX(float x);
    void SetPositionY(float y);
    void SetPosition(Vector2 newPos);
    void AddSpeedX(float delta);
    void ClampSpeed(float maxSpeed);
    
    Vector2 GetPosition();
    Vector2 GetSpeed();
    float GetRadius();
    bool IsMoving();
    bool IsOutOfScreen(int screenHeight);
    void SetSpeed(Vector2 newSpeed);
    Vector2 GetOriginalSpeed();
};

#endif