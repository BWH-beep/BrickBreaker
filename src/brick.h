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
    Color GetBrickColor(int index) const { return bricks[index].color; }
    void SetBrickColor(int index, Color c) { if (index < (int)bricks.size()) bricks[index].color = c; }
    bool IsBrickExplosive(int index) const { return bricks[index].isExplosive; }
    bool IsBrickEvil(int index) const { return bricks[index].isEvil; }
    void SetBrickType(int index, int type) {
        if (index < (int)bricks.size()) {
            bricks[index].active = (type != 0);
            bricks[index].isExplosive = (type == 2);
            bricks[index].isEvil = (type == 3);
        }
    }
    int GetBrickCount() const { return (int)bricks.size(); }
    bool IsBrickActive(int index) const { return bricks[index].active; }
    void SetBrickActive(int index, bool active) { if (index < (int)bricks.size()) bricks[index].active = active; }
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