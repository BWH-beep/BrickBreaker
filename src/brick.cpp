#include "brick.h"
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <fstream>
#include <nlohmann/json.hpp>
using json = nlohmann::json;

BrickManager::BrickManager(int width) {
    InitGrid(width, 600);  // 初始化网格
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
            if (brick.isExplosive) {
                float blink = (sin(time * 8.0f) + 1.0f) / 2.0f;
                Color drawColor = ColorAlpha(brick.color, 0.5f + 0.5f * blink);
                
                DrawRectangleRounded(
                    (Rectangle){brick.x, brick.y, brick.width, brick.height},
                    0.3f, 8, drawColor
                );
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
                
                float cx = brick.x + brick.width / 2;
                float cy = brick.y + brick.height / 2;
                DrawLine(cx - 6, cy - 6, cx + 6, cy + 6, RED);
                DrawLine(cx + 6, cy - 6, cx - 6, cy + 6, RED);
                DrawLine(cx - 7, cy - 7, cx + 7, cy + 7, RED);
                DrawLine(cx + 7, cy - 7, cx - 7, cy + 7, RED);
            }
            else if (brick.isEvil) {
                DrawRectangleRounded(
                    (Rectangle){brick.x, brick.y, brick.width, brick.height},
                    0.3f, 8, BLACK
                );
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, RED);
                
                float cx = brick.x + brick.width / 2;
                float cy = brick.y + brick.height / 2;
                DrawCircle(cx, cy - 2, 6, RED);
                DrawCircle(cx - 4, cy - 4, 2, WHITE);
                DrawCircle(cx + 4, cy - 4, 2, WHITE);
                DrawLine(cx - 3, cy + 2, cx + 3, cy + 2, WHITE);
            }
            else {
                Color baseColor = brick.color;
                
                DrawRectangleRounded(
                    (Rectangle){brick.x, brick.y, brick.width, brick.height},
                    0.3f, 8, Fade(baseColor, 0.75f)
                );
                
                DrawRectangleRounded(
                    (Rectangle){brick.x + 2, brick.y, brick.width - 4, brick.height * 0.45f},
                    0.2f, 8, Fade(WHITE, 0.25f)
                );
                
                DrawRectangleRounded(
                    (Rectangle){brick.x + 2, brick.y + brick.height * 0.55f, brick.width - 4, brick.height * 0.4f},
                    0.2f, 8, Fade(BLACK, 0.12f)
                );
                
                DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, Fade(WHITE, 0.4f));
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

/*bool BrickManager::CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score, 
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
}*/
bool BrickManager::CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score, 
                                   bool& dropPowerUp, Vector2& dropPos, bool& hitEvil, bool& hitExplosive) {
    hitEvil = false;
    hitExplosive = false;
    
    // 重建网格
    BuildGrid();
    
    // 确定球在哪个格子
    int gx = (int)(ballPos.x / cellWidth);
    int gy = (int)(ballPos.y / cellHeight);
    
    // 检测球所在格子及相邻格子
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            int nx = gx + dx;
            int ny = gy + dy;
            if (nx < 0 || nx >= GRID_COLS || ny < 0 || ny >= GRID_ROWS) continue;
            
            for (int idx : grid[nx][ny]) {
                Brick& brick = bricks[idx];
                if (!brick.active) continue;
                
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
                    
                    // 不可破坏砖块（障碍物）
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
    
    float brickW = 22.0f;
    float brickH = 14.0f;
    float gap = 2.0f;
    // 根据实际图案宽度自动居中
    float totalWidth = cols * (brickW + gap) - gap;
    float centerX = (screenWidth - totalWidth) / 2;
     TraceLog(LOG_INFO, "cols=%d, totalW=%.0f, centerX=%.0f, screenW=%d", cols, totalWidth, centerX, screenWidth);
    Color colors[] = { RED, ORANGE, YELLOW, GREEN, BLUE, PURPLE, PINK, SKYBLUE };
    
    for (int row = 0; row < rows; row++) {
        for (int col = 0; col < cols; col++) {
            if (pattern[row][col] == 0) continue;
            
            Brick brick;
            brick.x = centerX + col * (brickW + gap); 
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
void BrickManager::InitGrid(int screenW, int screenH) {
    cellWidth = (float)screenW / GRID_COLS;
    cellHeight = (float)screenH / GRID_ROWS;
}

void BrickManager::BuildGrid() {
    // 清空网格
    for (int i = 0; i < GRID_COLS; i++) {
        for (int j = 0; j < GRID_ROWS; j++) {
            grid[i][j].clear();
        }
    }
    
    // 把每个活跃砖块放入对应网格
    for (int idx = 0; idx < (int)bricks.size(); idx++) {
        if (!bricks[idx].active) continue;
        int gx = (int)(bricks[idx].x / cellWidth);
        int gy = (int)(bricks[idx].y / cellHeight);
        if (gx >= 0 && gx < GRID_COLS && gy >= 0 && gy < GRID_ROWS) {
            grid[gx][gy].push_back(idx);
        }
    }
}
//编辑模式
void BrickManager::ToggleBrickAt(float mouseX, float mouseY, float brickW, float brickH, float gap, float offsetY) {
    int col = (int)(mouseX / (brickW + gap));
    int row = (int)((mouseY - offsetY) / (brickH + gap));
    
    for (auto& brick : bricks) {
        int bc = (int)(brick.x / (brickW + gap));
        int br = (int)((brick.y - offsetY) / (brickH + gap));
        if (bc == col && br == row) {
            brick.active = !brick.active;
            return;
        }
    }
    
    Brick b;
    b.x = col * (brickW + gap);
    b.y = offsetY + row * (brickH + gap);
    b.width = brickW;
    b.height = brickH;
    b.active = true;
    b.color = RED;
    b.isEvil = false;
    b.isExplosive = false;
    b.indestructible = false;
    bricks.push_back(b);
}

void BrickManager::SaveLayoutToJSON(const std::string& path, float brickW, float brickH, float gap, float offsetY) {
    int maxRow = 0, maxCol = 0;
    for (auto& b : bricks) {
        int r = (int)((b.y - offsetY) / (brickH + gap));
        int c = (int)(b.x / (brickW + gap));
        if (r > maxRow) maxRow = r;
        if (c > maxCol) maxCol = c;
    }
    
    json j;
    for (int r = 0; r <= maxRow; r++) {
        json row;
        for (int c = 0; c <= maxCol; c++) {
            bool found = false;
            for (auto& b : bricks) {
                int br = (int)((b.y - offsetY) / (brickH + gap));
                int bc = (int)(b.x / (brickW + gap));
                if (br == r && bc == c && b.active) { row.push_back(1); found = true; break; }
            }
            if (!found) row.push_back(0);
        }
        j.push_back(row);
    }
    
    std::ofstream file(path);
    file << json{{"layout", j}}.dump(4);
}
void BrickManager::ToggleBrickAt(float mouseX, float mouseY, float brickW, float brickH, float gap, float offsetY, int type) {
    int col = (int)(mouseX / (brickW + gap));
    int row = (int)((mouseY - offsetY) / (brickH + gap));
    
    for (auto& brick : bricks) {
        int bc = (int)(brick.x / (brickW + gap));
        int br = (int)((brick.y - offsetY) / (brickH + gap));
        if (bc == col && br == row) {
            brick.active = !brick.active;
            return;
        }
    }
    
    Brick b;
    b.x = col * (brickW + gap);
    b.y = offsetY + row * (brickH + gap);
    b.width = brickW;
    b.height = brickH;
    b.active = true;
    b.isEvil = (type == 2);
    b.isExplosive = (type == 1);
    b.indestructible = (type == 3);
    if (type == 2) b.color = BLACK;
    else if (type == 3) b.color = GRAY;
    else if (type == 1) b.color = ORANGE;
    else b.color = RED;
    bricks.push_back(b);
}