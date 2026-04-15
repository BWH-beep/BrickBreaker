#include "ball.h"
#include <cmath>

Ball::Ball() {
    radius = 8;
    Reset();
}

void Ball::Reset() {
    position = { 400, 400 };
    speed = { 0, 0 };
    moving = false;
    trail.clear();
}

void Ball::Update(float dt) {
    if (moving) {
        // 记录当前位置到拖尾数组
        trail.push_back(position);
        // 保留最近20个位置，让尾巴更长
        if (trail.size() > 20) {
            trail.erase(trail.begin());
        }
        
        position.x += speed.x * dt;
        position.y += speed.y * dt;
    }
}

void Ball::Draw() {
    // 绘制拖尾虚影（历史位置）
    int trailSize = (int)trail.size();
    for (int i = 0; i < trailSize; i++) {
        float t = (float)i / trailSize;  // 0=最旧, 1=最新
        
        // 越旧越淡越小
        float alpha = 0.1f + 0.5f * t;
        float size = radius * (0.3f + 0.7f * t);
        
        // 绘制拖尾圆
        DrawCircleV(trail[i], size, Fade(YELLOW, alpha));
    }
    
    // 绘制当前球（带光晕）
    DrawCircleV(position, radius + 3, Fade(YELLOW, 0.2f));
    DrawCircleV(position, radius + 1, Fade(YELLOW, 0.4f));
    DrawCircleV(position, radius, YELLOW);
}

void Ball::Start(float speedX, float speedY) {
    moving = true;
    speed = { speedX, speedY };
    originalSpeed = speed;
}

void Ball::Stop() {
    moving = false;
}

void Ball::BounceX() {
    speed.x *= -1;
}

void Ball::BounceY() {
    speed.y *= -1;
}

void Ball::SetPositionX(float x) {
    position.x = x;
}

void Ball::SetPositionY(float y) {
    position.y = y;
}

void Ball::SetPosition(Vector2 newPos) {
    position = newPos;
}

void Ball::AddSpeedX(float delta) {
    speed.x += delta;
}

void Ball::ClampSpeed(float maxSpeed) {
    if (speed.x > maxSpeed) speed.x = maxSpeed;
    if (speed.x < -maxSpeed) speed.x = -maxSpeed;
    if (speed.y > maxSpeed) speed.y = maxSpeed;
    if (speed.y < -maxSpeed) speed.y = -maxSpeed;
}

Vector2 Ball::GetPosition() {
    return position;
}

Vector2 Ball::GetSpeed() {
    return speed;
}

float Ball::GetRadius() {
    return radius;
}

bool Ball::IsMoving() {
    return moving;
}

bool Ball::IsOutOfScreen(int screenHeight) {
    return position.y + radius > screenHeight;
}

void Ball::SetSpeed(Vector2 newSpeed) {
    speed = newSpeed;
}

Vector2 Ball::GetOriginalSpeed() {
    return originalSpeed;
}