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
            if (rand < 2) {  // 2% 邪恶砖块
                brick.isEvil = true;
                brick.color = BLACK;
            }
            else if (rand < 10) {  // 8% 爆炸砖块
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
                float blink = (sin(time * 8.0f) + 1.0f) / 2.0f;
                drawColor = ColorAlpha(brick.color, 0.5f + 0.5f * blink);
                DrawRectangle(brick.x, brick.y, brick.width, brick.height, drawColor);
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
                
                // 画叉叉
                float cx = brick.x + brick.width / 2;
                float cy = brick.y + brick.height / 2;
                DrawLine(cx - 6, cy - 6, cx + 6, cy + 6, RED);
                DrawLine(cx + 6, cy - 6, cx - 6, cy + 6, RED);
                DrawLine(cx - 7, cy - 7, cx + 7, cy + 7, RED);
                DrawLine(cx + 7, cy - 7, cx - 7, cy + 7, RED);
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
                
                // 画骷髅头
                float cx = brick.x + brick.width / 2;
                float cy = brick.y + brick.height / 2;
                DrawCircle(cx, cy - 2, 6, RED);
                DrawCircle(cx - 4, cy - 4, 2, WHITE);
                DrawCircle(cx + 4, cy - 4, 2, WHITE);
                DrawLine(cx - 3, cy + 2, cx + 3, cy + 2, WHITE);
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

void BrickManager::ExplodeArea(float centerX, float centerY, float radius, int& score, bool& dropPowerUp, Vector2& dropPos) {
    for (auto& brick : bricks) {
        if (!brick.active) continue;
        
        float brickCenterX = brick.x + brick.width / 2;
        float brickCenterY = brick.y + brick.height / 2;
        float dist = sqrt(pow(brickCenterX - centerX, 2) + pow(brickCenterY - centerY, 2));
        
        if (dist < radius) {
            brick.active = false;
            SpawnExplosion(brickCenterX, brickCenterY, brick.color);
            score += 10;
            
            if (GetRandomValue(0, 100) < 30) {
                dropPowerUp = true;
                dropPos = { brickCenterX, brickCenterY };
            }
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
                    
                    for (int i = 0; i < 50; i++) {
                        Particle p;
                        p.pos = { brick.x + brick.width/2, brick.y + brick.height/2 };
                        p.vel = { (float)(GetRandomValue(-600, 600)), (float)(GetRandomValue(-600, 600)) };
                        p.life = 0.8f;
                        p.color = ORANGE;
                        particles.push_back(p);
                    }
                    
                    ExplodeArea(brick.x + brick.width/2, brick.y + brick.height/2, 100.0f, score, dropPowerUp, dropPos);
                    ballSpeed.y *= -1;
                    score += 50;
                    return true;
                }
                
                // 不可破坏砖块（障碍物）：只反弹，不消失
                if (brick.indestructible) {
                    ballSpeed.y *= -1;
                    return true;
                }
                
                // 普通砖块
                brick.active = false;
                ballSpeed.y *= -1;
                score += 10;
                
                SpawnExplosion(brick.x + brick.width/2, brick.y + brick.height/2, brick.color);
                
                if (GetRandomValue(0, 100) < 30) {
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
        if (brick.active && !brick.indestructible) return false;
    }
    return true;
}
void BrickManager::LoadPattern(const std::vector<std::vector<int>>& pattern, int offsetX, int offsetY) {
    bricks.clear();
    
    int rows = (int)pattern.size();
    int cols = (int)pattern[0].size();
    
    float brickW = 40.0f;
    float brickH = 18.0f;
    float gap = 3.0f;
    
    Color colors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, SKYBLUE };
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (pattern[row][col] == 0) continue;
            
            Brick brick;
            brick.x = offsetX + col * (brickW + gap);
            brick.y = offsetY + row * (brickH + gap);
            brick.width = brickW;
            brick.height = brickH;
            brick.active = true;
            brick.indestructible = false;
            
            if (pattern[row][col] == 2) {
                brick.color = GRAY;
                brick.isEvil = false;
                brick.isExplosive = false;
                brick.indestructible = true;
            } else if (pattern[row][col] == 3) {
                brick.color = BLACK;
                brick.isEvil = true;
                brick.isExplosive = false;
            } else {
                brick.color = colors[row % 8];
                brick.isEvil = false;
                brick.isExplosive = false;
            }
            
            bricks.push_back(brick);
        }
    }
}