#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "raylib.h"
#include <cstdio>

class Game {
private:
    const int screenWidth;      // 屏幕宽度（800）
    const int screenHeight;     // 屏幕高度（600）
    
    Ball ball;                  // 球对象
    Paddle paddle;              // 板子对象
    BrickManager bricks;        // 砖块管理器对象
    Font chineseFont;           // 中文字体
      
    enum class GameState {
    WAITING,   // 等待开始
    PLAYING,   // 游戏中
    GAMEOVER,  // 游戏结束
    WIN        // 胜利
};

    GameState state;  // 当前游戏状态           // 是否胜利
    int score;       
    int lives;      // 当前得分和生命值
    
    const int STEPS = 120;       // 分段移动步数（防止穿透）

public:
    Game(int width, int height);    // 构造函数
    ~Game();                        // 析构函数
    void Run();                     // 游戏主循环（入口）

private:
    void ProcessInput();            // 处理键盘输入
    void Update(float dt);          // 更新游戏逻辑
    void Draw();                    // 绘制画面
    void Reset();                   // 重置游戏
    void LoadFont();                // 加载中文字体
    void DrawChineseText(const char* text, int x, int y, int fontSize, Color color);  // 绘制中文
    void DrawHearts(); 
};

#endif