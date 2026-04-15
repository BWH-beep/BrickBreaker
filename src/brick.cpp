#include "brick.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>

BrickManager::BrickManager(int width) {
    screenWidth = width;
    brickCols = 10;
    brickRows = 5;
    brickWidth = 70;
    brickHeight = 25;
    spacing = 5;
    Reset();
}

void BrickManager::Reset() {
    bricks.clear();
    particles.clear();
    float startX = (screenWidth - (brickCols * (brickWidth + spacing))) / 2;
    float startY = 80;
    Color rowColors[] = { RED, ORANGE, YELLOW, GREEN, BLUE };
    
    for (int row = 0; row < brickRows; row++) {
        for (int col = 0; col < brickCols; col++) {
            Brick brick;
            brick.x = startX + col * (brickWidth + spacing);
            brick.y = startY + row * (brickHeight + spacing);
            brick.width = brickWidth;
            brick.height = brickHeight;
            brick.active = true;
            brick.color = rowColors[row % 5];
            brick.isEvil = false;
            
            if (GetRandomValue(0, 100) < 3) {
                brick.isEvil = true;
                brick.color = BLACK;
            }
            
            bricks.push_back(brick);
        }
    }
}

void BrickManager::Draw() {
    float time = GetTime();
    
    for (const auto& brick : bricks) {
        if (brick.active) {
            Color drawColor = brick.color;
            
            if (!brick.isEvil) {
                float blink = (sin(time * 5.0f) + 1.0f) / 2.0f;
                drawColor = ColorAlpha(brick.color, 0.6f + 0.4f * blink);
            }
            
            DrawRectangle(brick.x, brick.y, brick.width, brick.height, drawColor);
            
            if (brick.isEvil) {
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
            } else {
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, WHITE);
            }
        }
    }
    
    for (const auto& p : particles) {
        DrawCircleV(p.pos, 3, p.color);
    }
}

void BrickManager::UpdateParticles(float dt) {
    for (int i = particles.size() - 1; i >= 0; i--) {
        particles[i].pos.x += particles[i].vel.x * dt;
        particles[i].pos.y += particles[i].vel.y * dt;
        particles[i].life -= dt;
        if (particles[i].life <= 0) {
            particles.erase(particles.begin() + i);
        }
    }
}

void BrickManager::SpawnExplosion(float x, float y, Color color) {
    // 生成爆炸粒子
    for (int i = 0; i < 20; i++) {
        Particle p;
        p.pos = { x, y };
        p.vel = { (float)(GetRandomValue(-400, 400)), (float)(GetRandomValue(-400, 400)) };
        p.life = 0.6f;
        p.color = color;
        particles.push_back(p);
    }
    
    // 额外白色/黄色高亮粒子
    for (int i = 0; i < 10; i++) {
        Particle p;
        p.pos = { x + GetRandomValue(-15, 15), y + GetRandomValue(-15, 15) };
        p.vel = { (float)(GetRandomValue(-300, 300)), (float)(GetRandomValue(-300, 300)) };
        p.life = 0.4f;
        p.color = (Color){ 255, 200, 50, 255 };
        particles.push_back(p);
    }
}

bool BrickManager::CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score, 
                                   bool& dropPowerUp, Vector2& dropPos, bool& hitEvil) {
    for (auto& brick : bricks) {
        if (brick.active) {
            Rectangle brickRect = { brick.x, brick.y, brick.width, brick.height };
            if (CheckCollisionCircleRec(ballPos, ballRadius, brickRect)) {
                if (brick.isEvil) {
                    hitEvil = true;
                    brick.active = false;
                    SpawnExplosion(brick.x + brick.width/2, brick.y + brick.height/2, RED);
                    return true;
                }
                brick.active = false;
                ballSpeed.y *= -1;
                score += 10;
                
                // ========== 添加破碎特效 ==========
                SpawnExplosion(brick.x + brick.width/2, brick.y + brick.height/2, brick.color);
                // =================================
                
                if (GetRandomValue(0, 100) < 50) {
                    dropPowerUp = true;
                    dropPos = { brick.x + brick.width/2, brick.y + brick.height/2 };
                }
                return true;
            }
        }
    }
    return false;
}

bool BrickManager::AllCleared() {
    for (const auto& brick : bricks) {
        if (brick.active) return false;
    }
    return true;
}