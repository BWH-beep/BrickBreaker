#ifndef BALL_H
#define BALL_H

#include "game_object.h"
#include <vector>

class Ball : public PhysicalObject, public VisualObject {
private:
    bool moving;
    Vector2 originalSpeed;
    std::vector<Vector2> trail;
    bool invincible;
    float invincibleTimer;
    int scoreValue;  // 新增：击中砖块得分

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
    void AddSpeedX(float delta);
    void ClampSpeed(float maxSpeed);
    
    void SetInvincible(bool inv, float duration);
    bool IsInvincible() { return invincible; }
    void UpdateInvincible(float dt);
    void RecordTrail();
    
    // 使用基类方法时注意：GetPosition() 来自 GameObject
    // GetVelocity() 来自 PhysicalObject
    // GetRadius() 来自 PhysicalObject
    
    bool IsMoving();
    bool IsOutOfScreen(int screenHeight);
    void SetSpeed(Vector2 newSpeed);
    Vector2 GetOriginalSpeed();
    int GetScoreValue() const { return scoreValue; }
    void SetScoreValue(int val) { scoreValue = val; }
};

#endif