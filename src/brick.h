#ifndef BRICK_H
#define BRICK_H

#include "raylib.h"
#include <vector>

// 粒子结构
struct Particle {
    Vector2 pos;
    Vector2 vel;
    float life;
    Color color;
};

// 单个砖块的结构体
struct Brick {
    float x, y;
    float width, height;
    bool active;
    Color color;
    bool isEvil;
};

// 砖块管理器类
class BrickManager {
private:
    std::vector<Brick> bricks;
    std::vector<Particle> particles;  // 粒子系统
    int brickCols;
    int brickRows;
    float brickWidth;
    float brickHeight;
    float spacing;
    int screenWidth;

public:
    BrickManager(int width);
    void Reset();
    void Draw();
    void UpdateParticles(float dt);  // 更新粒子
    void SpawnExplosion(float x, float y, Color color);  // 爆炸特效
    bool CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score,
                        bool& dropPowerUp, Vector2& dropPos, bool& hitEvil);
    bool AllCleared();
};

#endif