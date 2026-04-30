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
    float time = GetTime();
    float displayRadius = invincible ? radius + 4 : radius - 1;
    
    Color ballColor;
    if (invincible) {
        ballColor.r = (unsigned char)((sin(time * 4.0f) * 0.5f + 0.5f) * 255);
        ballColor.g = (unsigned char)((sin(time * 4.0f + 2.0f) * 0.5f + 0.5f) * 255);
        ballColor.b = (unsigned char)((sin(time * 4.0f + 4.0f) * 0.5f + 0.5f) * 255);
        ballColor.a = 255;
    } else {
        ballColor = (Color){ 255, 220, 50, 255 };
    }
    
    // 拖尾
    if (moving) {
        Vector2 dir = speed;
        float spd = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (spd > 0.1f) {
            dir.x /= spd;
            dir.y /= spd;
            
            for (int i = 1; i <= 12; i++) {
                float t = (float)i / 12.0f;
                float dist = i * 5.0f;
                float alpha = (1.0f - t) * 0.6f;
                float size = displayRadius * (1.0f - t * 0.7f);
                
                Vector2 trailPos = {
                    position.x - dir.x * dist,
                    position.y - dir.y * dist
                };
                
                Color tColor = ballColor;
                tColor.a = (unsigned char)(alpha * 255);
                DrawCircleV(trailPos, size, Fade(tColor, alpha));
            }
        }
    }
    
    // 球体光晕
    DrawCircleV(position, displayRadius + 5, Fade(ballColor, 0.2f));
    DrawCircleV(position, displayRadius + 2, Fade(ballColor, 0.4f));
    
    // 主球体
    DrawCircleV(position, displayRadius, ballColor);
    
    // 高光
    DrawCircleV(
        (Vector2){position.x - displayRadius * 0.25f, position.y - displayRadius * 0.3f},
        displayRadius * 0.5f, Fade(WHITE, 0.35f)
    );
    DrawCircleV(
        (Vector2){position.x - displayRadius * 0.15f, position.y - displayRadius * 0.2f},
        displayRadius * 0.2f, WHITE
    );
    
    // 无敌旋转光环
    if (invincible) {
        for (int i = 0; i < 4; i++) {
            float angle = time * 5.0f + i * 90.0f * DEG2RAD;
            Vector2 starPos = {
                position.x + cos(angle) * (displayRadius + 12),
                position.y + sin(angle) * (displayRadius + 12)
            };
            DrawCircleV(starPos, 4, GOLD);
        }
    }
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
