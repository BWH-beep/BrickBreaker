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
        ballColor = YELLOW;
    }
    
    // ========== 实心渐变拖尾 ==========
    if (moving) {
        Vector2 dir = speed;
        float spd = sqrt(dir.x * dir.x + dir.y * dir.y);
        if (spd > 0.1f) {
            dir.x /= spd;
            dir.y /= spd;
            
            Vector2 perp = { -dir.y, dir.x };
            
            // 画多层叠加的线段，形成实心渐变效果
            int layers = 12;  // 层数
            for (int layer = 0; layer < layers; layer++) {
                float layerT = (float)layer / layers;  // 0 = 边缘, 1 = 中心
                float offset = displayRadius * (1.0f - layerT);  // 从边缘到中心
                
                // 每层画一段拖尾
                int segments = 20;
                for (int i = 0; i < segments; i++) {
                    float t = (float)i / segments;
                    float dist = i * 5.0f;
                    
                    // 透明度：边缘淡，中心更淡；越远越淡
                    float edgeAlpha = 0.15f * (1.0f - layerT);  // 边缘透明度
                    float distAlpha = 1.0f - t;  // 距离衰减
                    float alpha = (edgeAlpha + 0.1f) * distAlpha;
                    
                    // 粗细：边缘细，中心粗
                    float thickness = 1.5f + (1.0f - layerT) * 3.0f;
                    thickness *= (1.0f - t * 0.5f);  // 越远越细
                    
                    Vector2 backPos = {
                        position.x - dir.x * dist,
                        position.y - dir.y * dist
                    };
                    Vector2 backPosNext = {
                        position.x - dir.x * (dist + 5.0f),
                        position.y - dir.y * (dist + 5.0f)
                    };
                    
                    // 颜色渐变
                    Color trailColor;
                    if (invincible) {
                        trailColor = ballColor;
                    } else {
                        trailColor.r = 255;
                        trailColor.g = (unsigned char)(200 + 55 * t);
                        trailColor.b = (unsigned char)(80 + 80 * t);
                        trailColor.a = 255;
                    }
                    
                    // 左侧
                    Vector2 left1 = { backPos.x + perp.x * offset, backPos.y + perp.y * offset };
                    Vector2 left2 = { backPosNext.x + perp.x * offset, backPosNext.y + perp.y * offset };
                    DrawLineEx(left1, left2, thickness, Fade(trailColor, alpha));
                    
                    // 右侧
                    Vector2 right1 = { backPos.x - perp.x * offset, backPos.y - perp.y * offset };
                    Vector2 right2 = { backPosNext.x - perp.x * offset, backPosNext.y - perp.y * offset };
                    DrawLineEx(right1, right2, thickness, Fade(trailColor, alpha));
                }
            }
            
            // 中心填充：画一串半透明的圆
            for (int i = 0; i < 15; i++) {
                float t = (float)i / 15.0f;
                float dist = i * 6.0f;
                float alpha = (1.0f - t) * 0.2f;
                float size = displayRadius * 0.7f * (1.0f - t * 0.5f);
                
                Vector2 trailPos = {
                    position.x - dir.x * dist,
                    position.y - dir.y * dist
                };
                
                Color fillColor;
                if (invincible) {
                    fillColor = ballColor;
                } else {
                    fillColor.r = 255;
                    fillColor.g = (unsigned char)(220 + 35 * t);
                    fillColor.b = (unsigned char)(100 + 50 * t);
                    fillColor.a = 255;
                }
                
                DrawCircleV(trailPos, size, Fade(fillColor, alpha));
            }
        }
    }
    
    // 绘制球体
    DrawCircleV(position, displayRadius + 4, Fade(ballColor, 0.15f));
    DrawCircleV(position, displayRadius + 2, Fade(ballColor, 0.3f));
    DrawCircleV(position, displayRadius, ballColor);
    
    // 高光
    Vector2 highlight = { position.x - displayRadius * 0.25f, position.y - displayRadius * 0.25f };
    DrawCircleV(highlight, displayRadius * 0.45f, Fade(WHITE, 0.25f));
    DrawCircleV(highlight, displayRadius * 0.2f, Fade(WHITE, 0.5f));
    DrawCircleV(position, displayRadius * 0.1f, WHITE);
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
