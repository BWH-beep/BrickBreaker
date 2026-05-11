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
    bool indestructible;  // 新增：不可破坏
};

class BrickManager {
private:
    // 空间划分网格
    static const int GRID_COLS = 8;
    static const int GRID_ROWS = 6;
    float cellWidth;
    float cellHeight;
    std::vector<int> grid[GRID_COLS][GRID_ROWS];  // 每个格子存砖块索引
    
    void BuildGrid();  // 每帧重建网格
    std::vector<Brick> bricks;
    std::vector<Particle> particles;
    int brickCols;
    int brickRows;
    float brickWidth;
    float brickHeight;
    float spacing;
    int screenWidth;

public:
    void InitGrid(int screenW, int screenH);  // 初始化网格参数
    void LoadPattern(const std::vector<std::vector<int>>& pattern, int offsetX, int offsetY);
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