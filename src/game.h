#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "raylib.h"
#include "Config.h" 
#include <cstdio>
#include <vector>  // 用于存储多个道具和多个球

class Game {
private:
    Texture2D background;  // 声明一个纹理变量，用来存储图片
    const int screenWidth;      // 屏幕宽度（800）
    const int screenHeight;     // 屏幕高度（600）
    Config config;                  
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
    
    int steps;      // 分段移动步数（防止穿透）
    // ========== 道具系统 ==========
    struct PowerUp {
    Vector2 position;  // 道具位置
    bool active;       // 是否还在屏幕上
    int type;          // 0=加长板子,1=缩短板子,2=多球,3=慢速球,4=加分
    float speedY;      // 下落速度
};

   std::vector<PowerUp> powerUps;  // 存储所有道具
   float powerUpTimer[5];          // 计时器数组（每个道具类型一个）
   float originalPaddleWidth;      // 保存板子原始宽度
   std::vector<Ball> balls;        // 多个球（用于多球道具）
// ============================

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
    // 道具系统函数
    void SpawnPowerUp(Vector2 pos, int type);   // 生成道具
    void ApplyPowerUp(int type);                // 应用道具效果
    void UpdatePowerUps(float dt);              // 更新道具（移动、碰撞、计时）
    void DrawPowerUps();                        // 绘制道具
};

#endif