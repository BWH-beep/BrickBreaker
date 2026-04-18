#ifndef BRICK_H
#define BRICK_H

#include "raylib.h"
#include <vector>

struct Particle {
    Vector2 pos;
    Vector2 vel;
    float life;
    Color color;
};

struct Brick {
    float x, y;
    float width, height;
    bool active;
    Color color;
    bool isEvil;
    bool isExplosive;
};

class BrickManager {
private:
    std::vector<Brick> bricks;
    std::vector<Particle> particles;
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
    void UpdateParticles(float dt);
    void SpawnExplosion(float x, float y, Color color);
    bool CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score,
                        bool& dropPowerUp, Vector2& dropPos, bool& hitEvil, bool& hitExplosive);
    bool AllCleared();
    void SetRows(int rows);
    void ExplodeArea(float centerX, float centerY, float radius, int& score, bool& dropPowerUp, Vector2& dropPos);
};

#endif