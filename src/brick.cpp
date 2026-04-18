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
            brick.isExplosive = false;
            
            int rand = GetRandomValue(0, 100);
            if (rand < 5) {
                brick.isEvil = true;
                brick.color = BLACK;
            }
            else if (rand < 35) {  // 30% 爆炸砖块
                brick.isExplosive = true;
                brick.color = ORANGE;
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
            
            if (brick.isExplosive) {
                // 爆炸砖块闪烁
                float blink = (sin(time * 8.0f) + 1.0f) / 2.0f;
                drawColor = ColorAlpha(brick.color, 0.5f + 0.5f * blink);
                DrawRectangle(brick.x, brick.y, brick.width, brick.height, drawColor);
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
                // 绘制爆炸标志
                DrawText("X", brick.x + brick.width/2 - 8, brick.y + 5, 20, RED);
            }
            else if (!brick.isEvil) {
                float blink = (sin(time * 5.0f) + 1.0f) / 2.0f;
                drawColor = ColorAlpha(brick.color, 0.6f + 0.4f * blink);
                DrawRectangle(brick.x, brick.y, brick.width, brick.height, drawColor);
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, WHITE);
            }
            
            if (brick.isEvil) {
                DrawRectangle(brick.x, brick.y, brick.width, brick.height, BLACK);
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
                DrawText("!", brick.x + brick.width/2 - 5, brick.y + 5, 20, RED);
            }
        }
    }
    
    for (const auto& p : particles) {
        DrawCircleV(p.pos, 3, p.color);
    }
}

void BrickManager::SetRows(int rows) {
    brickRows = rows;
    Reset();
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
    for (int i = 0; i < 20; i++) {
        Particle p;
        p.pos = { x, y };
        p.vel = { (float)(GetRandomValue(-400, 400)), (float)(GetRandomValue(-400, 400)) };
        p.life = 0.6f;
        p.color = color;
        particles.push_back(p);
    }
    
    for (int i = 0; i < 10; i++) {
        Particle p;
        p.pos = { x + GetRandomValue(-15, 15), y + GetRandomValue(-15, 15) };
        p.vel = { (float)(GetRandomValue(-300, 300)), (float)(GetRandomValue(-300, 300)) };
        p.life = 0.4f;
        p.color = (Color){ 255, 200, 50, 255 };
        particles.push_back(p);
    }
}

void BrickManager::ExplodeArea(float centerX, float centerY, float radius) {
    for (auto& brick : bricks) {
        if (!brick.active) continue;
        
        float brickCenterX = brick.x + brick.width / 2;
        float brickCenterY = brick.y + brick.height / 2;
        float dist = sqrt(pow(brickCenterX - centerX, 2) + pow(brickCenterY - centerY, 2));
        
        if (dist < radius) {
            brick.active = false;
            SpawnExplosion(brickCenterX, brickCenterY, brick.color);
        }
    }
}

bool BrickManager::CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score, 
                                   bool& dropPowerUp, Vector2& dropPos, bool& hitEvil, bool& hitExplosive) {
    hitEvil = false;
    hitExplosive = false;
    
    for (auto& brick : bricks) {
        if (brick.active) {
            Rectangle brickRect = { brick.x, brick.y, brick.width, brick.height };
            if (CheckCollisionCircleRec(ballPos, ballRadius, brickRect)) {
                // 邪恶砖块
                if (brick.isEvil) {
                    hitEvil = true;
                    brick.active = false;
                    SpawnExplosion(brick.x + brick.width/2, brick.y + brick.height/2, RED);
                    return true;
                }
                
                // 爆炸砖块
                if (brick.isExplosive) {
                    hitExplosive = true;
                    brick.active = false;
                    
                    // 大爆炸特效
                    for (int i = 0; i < 50; i++) {
                        Particle p;
                        p.pos = { brick.x + brick.width/2, brick.y + brick.height/2 };
                        p.vel = { (float)(GetRandomValue(-600, 600)), (float)(GetRandomValue(-600, 600)) };
                        p.life = 0.8f;
                        p.color = ORANGE;
                        particles.push_back(p);
                    }
                    
                    // 炸毁周围砖块
                    ExplodeArea(brick.x + brick.width/2, brick.y + brick.height/2, 100.0f);
                    ballSpeed.y *= -1;
                    score += 50;
                    return true;
                }
                
                // 普通砖块
                brick.active = false;
                ballSpeed.y *= -1;
                score += 10;
                
                SpawnExplosion(brick.x + brick.width/2, brick.y + brick.height/2, brick.color);
                
                // 60% 掉落道具
                if (GetRandomValue(0, 100) < 60) {
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