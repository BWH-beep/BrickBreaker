#ifndef NETWORK_H
#define NETWORK_H

#pragma pack(push, 1)
struct GameState {
    float paddle1Width;  // 主机板宽度
    float paddle2Width;  // 客户端板宽度
    float ballX, ballY;
    float ballSpeedX, ballSpeedY;
    float paddle1X, paddle2X;
    int score1, score2;
    int lives1, lives2;
    int brickType[50];  // 0=已碎, 1=普通, 2=爆炸, 3=恶魔
};
#pragma pack(pop)

#endif