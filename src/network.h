#ifndef NETWORK_H
#define NETWORK_H

#pragma pack(push, 1)
struct GameState {
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float paddle1X, paddle2X;
    int score1, score2;
    int lives1, lives2;
    int brickActive[50];  // 砖块激活状态（1=激活，0=已碎）
};
#pragma pack(pop)

#endif