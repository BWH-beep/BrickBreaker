#ifndef GAME_H
#define GAME_H

#include "ball.h"
#include "paddle.h"
#include "brick.h"
#include "raylib.h"
#include "Config.h" 
#include <cstdio>
#include <vector>
#include <algorithm>
#include "network_manager.h"

// 特效粒子结构
struct EffectParticle {
    Vector2 pos;
    Vector2 vel;
    float life;
    float size;
    Color color;
};

// 闪光特效
struct FlashEffect {
    float intensity;
    float life;
};

// 文字飘浮特效
struct FloatingText {
    Vector2 pos;
    char text[30];
    float life;
    Color color;
};

class Game {
private:
    Rectangle menuButton;          // 暂停菜单：返回主菜单按钮
    bool backToMenu;
    int currentLevel;   
    int currentDifficulty;           // 当前关卡
    std::vector<std::vector<int>> levelPatterns;  // 关卡图案
    void InitLevels();             // 初始化关卡
    void NextLevel();              // 下一关
    void LoadLevel(int level);     // 加载指定关卡
    float clientPaddleX = 400;   // 客户端板位置（主机用）
    float hostPaddleX = 400;     // 主机板位置（客户端用）
    NetworkManager network;          // 网络管理器
    NetworkMode networkMode;         // 当前网络模式
    bool isNetworkGame;              // 是否联机模式
    void UpdateNetwork();           // 网络更新（主机发送，客户端接收）
    void SendGameState();           // 主机发送游戏状态
    void ReceiveGameState();        // 客户端接收游戏状态
    Texture2D powerUpTexture;
    Texture2D background;
    const int screenWidth;
    const int screenHeight;
    Config config;                  
    Paddle paddle;
    BrickManager bricks;
    Font chineseFont;
      
   enum class GameState {
    WAITING,
    PLAYING,
    GAMEOVER,
    WIN,
    LEVEL_COMPLETE,/*过关和失败选择界面*/
    GAME_OVER_MENU
};

    GameState state;
    int score;       
    int lives;
    int steps;
    
    // 道具系统
    struct PowerUp {
        Vector2 position;
        bool active;
        int type;  // 0:加长, 1:缩短, 2:多球, 3:慢速, 4:加分, 5:无敌
        float speedY;
    };
    
    std::vector<PowerUp> powerUps;
    float powerUpTimer[6];  // 增加到6个
    float originalPaddleWidth;
    std::vector<Ball> balls;
    
    // 特效系统
    std::vector<EffectParticle> effectParticles;
    std::vector<FloatingText> floatingTexts;
    FlashEffect flash;
    float shakeTime;

public:
    Game(int width, int height);
    ~Game();
    void Run();

private:
    void ProcessInput();
    void Update(float dt);
    void Draw();
    void Reset();
    void LoadFont();
    void DrawChineseText(const char* text, int x, int y, int fontSize, Color color);
    void DrawHearts();
    void DrawPowerUps();
    void SpawnPowerUp(Vector2 pos, int type);
    void ApplyPowerUp(int type);
    void UpdatePowerUps(float dt);
    bool paused;                    // 是否暂停
    Rectangle pauseButton;          // 暂停按钮区域
    Rectangle continueButton;       // 继续按钮
    Rectangle quitButton;           // 退出按钮
};

#endif