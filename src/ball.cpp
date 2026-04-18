#include "ball.h"
#include <cmath>
#include <cstdio>

Ball::Ball() {
    radius = 12;
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
    radius = 12;  // 明显变大
    // ================================
}

void Ball::Update(float dt) {
    if (invincible) {
        UpdateInvincible(dt);
    }
    
    if (moving) {
        position.x += speed.x * dt;
        position.y += speed.y * dt;
    }
}

void Ball::RecordTrail() {
    if (moving) {
        trail.push_back(position);
        if (trail.size() > 40) {
            trail.erase(trail.begin());
        }
    }
}
void Ball::UpdateInvincible(float dt) {
    if (invincibleTimer > 0) {
        invincibleTimer -= dt;
        if (invincibleTimer <= 0) {
            invincible = false;
            radius = 12;
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
        radius = 12;
    }
}

void Ball::Draw() {
    // 确定球的颜色
    Color ballColor;
    if (invincible) {
        float time = GetTime();
        int r = (int)((sin(time * 3.0f) * 0.5f + 0.5f) * 255);
        int g = (int)((sin(time * 3.0f + 2.0f) * 0.5f + 0.5f) * 255);
        int b = (int)((sin(time * 3.0f + 4.0f) * 0.5f + 0.5f) * 255);
        ballColor = (Color){ (unsigned char)r, (unsigned char)g, (unsigned char)b, 255 };
        
        // 无敌光环
        DrawCircleV(position, radius + 15, Fade(ballColor, 0.2f));
        DrawCircleV(position, radius + 10, Fade(ballColor, 0.3f));
        DrawCircleV(position, radius + 5, Fade(WHITE, 0.4f));
    } else {
        ballColor = YELLOW;
    }
    
    // ========== 绘制彗星拖尾 ==========
    int trailSize = (int)trail.size();
    for (int i = 0; i < trailSize; i++) {
        float t = (float)i / trailSize;  // 0 = 最旧（尾巴末端）, 1 = 最新（靠近球）
        
        // 越靠近球：越大、越亮
        float size = radius * (0.3f + 0.7f * t);
        float alpha = 0.2f + 0.8f * t;
        
        // 拖尾颜色渐变
        Color trailColor;
        if (invincible) {
            // 无敌时彩虹拖尾
            float time = GetTime();
            trailColor.r = (unsigned char)((sin(time * 3.0f + i * 0.1f) * 0.5f + 0.5f) * 255);
            trailColor.g = (unsigned char)((sin(time * 3.0f + 2.0f + i * 0.1f) * 0.5f + 0.5f) * 255);
            trailColor.b = (unsigned char)((sin(time * 3.0f + 4.0f + i * 0.1f) * 0.5f + 0.5f) * 255);
            trailColor.a = 255;
        } else {
            // 普通球：黄色渐变到橙红
            trailColor.r = 255;
            trailColor.g = (unsigned char)(180 + 75 * t);
            trailColor.b = (unsigned char)(50 * (1 - t));
            trailColor.a = 255;
        }
        
        // 画拖尾主圆
        DrawCircleV(trail[i], size, Fade(trailColor, alpha));
        
        // 画拖尾内层高光（更亮）
        DrawCircleV(trail[i], size * 0.6f, Fade(WHITE, alpha * 0.4f));
    }
    
    // ========== 绘制球体 ==========
    // 外层光晕
    DrawCircleV(position, radius + 5, Fade(ballColor, 0.2f));
    DrawCircleV(position, radius + 2, Fade(ballColor, 0.4f));
    
    // 主球体
    DrawCircleV(position, radius, ballColor);
    
    // 球体高光（模拟立体感）
    DrawCircleV(position, radius * 0.5f, Fade(WHITE, 0.5f));
    
    // 球中心最亮
    DrawCircleV(position, 2, WHITE);
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
