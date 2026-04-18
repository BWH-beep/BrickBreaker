#include "ball.h"
#include <cmath>
#include <cstdio>

Ball::Ball() {
    radius = 8;
    invincible = false;
    invincibleTimer = 0;
    Reset();
}

void Ball::Reset() {
    position = { 400, 400 };
    speed = { 0, 0 };
    moving = false;
    trail.clear();
    
    // ========== 强制无敌测试 ==========
    invincible = false;
    invincibleTimer = 0.0f;  // 很长时间
    radius = 16;  // 明显变大
    // ================================
}

void Ball::Update(float dt) {
    if (invincible) {
        UpdateInvincible(dt);
    }
    
    if (moving) {
        trail.push_back(position);
        if (trail.size() > 20) {
            trail.erase(trail.begin());
        }
        
        position.x += speed.x * dt;
        position.y += speed.y * dt;
    }
}

void Ball::UpdateInvincible(float dt) {
    if (invincibleTimer > 0) {
        invincibleTimer -= dt;
        if (invincibleTimer <= 0) {
            invincible = false;
            radius = 8;
        }
    }
}

void Ball::SetInvincible(bool inv, float duration) {
    invincible = inv;
    if (inv) {
        invincibleTimer = duration;
        radius = 16;
    } else {
        invincibleTimer = 0;
        radius = 8;
    }
}

void Ball::Draw() {
    // 无敌时强制使用亮红色，确保能看到变化
    Color ballColor;
    if (invincible) {
        // 直接用明显的颜色变化
        float time = GetTime();
        int r = (int)((sin(time * 3.0f) * 0.5f + 0.5f) * 255);
        int g = (int)((sin(time * 3.0f + 2.0f) * 0.5f + 0.5f) * 255);
        int b = (int)((sin(time * 3.0f + 4.0f) * 0.5f + 0.5f) * 255);
        ballColor = (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
        
        // 画一个大光环
        DrawCircleV(position, radius + 15, Fade(ballColor, 0.2f));
        DrawCircleV(position, radius + 10, Fade(ballColor, 0.3f));
        DrawCircleV(position, radius + 5, Fade(WHITE, 0.4f));
    } else {
        ballColor = YELLOW;
    }
    
    // 绘制拖尾
    int trailSize = (int)trail.size();
    for (int i = 0; i < trailSize; i++) {
        float t = (float)i / trailSize;
        float alpha = 0.1f + 0.5f * t;
        float size = radius * (0.3f + 0.7f * t);
        DrawCircleV(trail[i], size, Fade(ballColor, alpha));
    }
    
    // 绘制球体
    DrawCircleV(position, radius + 3, Fade(ballColor, 0.3f));
    DrawCircleV(position, radius + 1, Fade(ballColor, 0.5f));
    DrawCircleV(position, radius, ballColor);
    
    // 球中心画个白色亮点
    DrawCircleV(position, 3, WHITE);
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
    if (invincible) {
        // 无敌时打印调试信息
        static int frameCount = 0;
        if (frameCount++ % 60 == 0) {
            printf("Ball is invincible! Radius: %f\n", radius);
        }
        return false;
    }
    return position.y + radius > screenHeight;
}

void Ball::SetSpeed(Vector2 newSpeed) {
    speed = newSpeed;
}

Vector2 Ball::GetOriginalSpeed() {
    return originalSpeed;
}