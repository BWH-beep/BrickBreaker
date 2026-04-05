#ifndef BALL_H
#define BALL_H

#include "raylib.h"

class Ball {
private:
    Vector2 position;
    Vector2 speed;
    float radius;
    bool moving;

public:
    Ball();
    void Update(float dt);
    void Draw();
    void Start();
    void Stop();
    void Reset();
    void BounceX();
    void BounceY();
    void SetPositionX(float x);
    void SetPositionY(float y);
    void SetPosition(Vector2 newPos);
    void SetSpeed(Vector2 newSpeed);
    void AddSpeedX(float delta);
    void ClampSpeed(float maxSpeed);
    
    Vector2 GetPosition();
    Vector2 GetSpeed();
    float GetRadius();
    bool IsMoving();
    bool IsOutOfScreen(int screenHeight);
};

#endif