#ifndef NETWORK_H
#define NETWORK_H

#include "raylib.h"

#pragma pack(push, 1)
struct GameState {
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float paddle1X, paddle2X;
    float paddle1Width, paddle2Width;
    int score1, score2;
    int lives1, lives2;
    int brickType[50];
    Color brickColor[50];
    
    // 特效同步
    int ballCount;              // 球的数量
    float ballX2[12];           // 多球位置（最多12个）
    float ballY2[12];
    float ballSpeedX2[12];
    float ballSpeedY2[12];
    bool isInvincible;          // 无敌状态
    float invincibleTimer;      // 无敌剩余时间
    int activePowerUps[3];      // 当前活跃的道具类型
    float powerUpTimers[3];     // 道具剩余时间
};
#pragma pack(pop)

#endif