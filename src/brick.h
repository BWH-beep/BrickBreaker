#ifndef BRICK_H
#define BRICK_H

#include "raylib.h"
#include <vector>

// 单个砖块的结构体
struct Brick {
    float x, y;          // 砖块左上角坐标
    float width, height; // 砖块宽高
    bool active;         // 砖块是否还存在（true=存在，false=已被打掉）
    Color color;         // 砖块颜色
};

// 砖块管理器类
class BrickManager {
private:
    std::vector<Brick> bricks;  // 存储所有砖块的容器
    int brickCols;              // 砖块列数（10列）
    int brickRows;              // 砖块行数（5行）
    float brickWidth;           // 单个砖块宽度（70像素）
    float brickHeight;          // 单个砖块高度（25像素）
    float spacing;              // 砖块之间的间距（5像素）
    int screenWidth;            // 屏幕宽度（用于计算起始位置）

public:
    BrickManager(int width);            // 构造函数
    void Reset();                       // 重置/创建所有砖块
    void Draw();                        // 绘制所有砖块
    bool CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score);  // 碰撞检测
    bool AllCleared();                  // 检查是否所有砖块都被打掉了
};

#endif