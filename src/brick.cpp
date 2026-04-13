#include "brick.h"
#include <cmath>
#include <cstdio>

BrickManager::BrickManager(int width) {
    screenWidth = width;
    brickCols = 10;
    brickRows = 5;
    brickWidth = 70;
    brickHeight = 25;
    spacing = 5;
    Reset();
}

void BrickManager::Reset() {
    bricks.clear();
    float startX = (screenWidth - (brickCols * (brickWidth + spacing))) / 2;
    float startY = 80;
    Color rowColors[] = { RED, ORANGE, YELLOW, GREEN, BLUE };
    
    for (int row = 0; row < brickRows; row++) {
        for (int col = 0; col < brickCols; col++) {
            Brick brick;
            brick.x = startX + col * (brickWidth + spacing);
            brick.y = startY + row * (brickHeight + spacing);
            brick.width = brickWidth;
            brick.height = brickHeight;
            brick.active = true;
            brick.color = rowColors[row % 5];
            bricks.push_back(brick);
        }
    }
}

void BrickManager::Draw() {
    for (const auto& brick : bricks) {
        if (brick.active) {
            DrawRectangle(brick.x, brick.y, brick.width, brick.height, brick.color);
            DrawRectangleLines(brick.x, brick.y, brick.width, brick.height, WHITE);
        }
    }
}

bool BrickManager::CheckCollision(Vector2 ballPos, float ballRadius, Vector2& ballSpeed, int& score, 
                                   bool& dropPowerUp, Vector2& dropPos) {
    for (auto& brick : bricks) {
        if (brick.active) {
            Rectangle brickRect = { brick.x, brick.y, brick.width, brick.height };
            if (CheckCollisionCircleRec(ballPos, ballRadius, brickRect)) {
                brick.active = false;
                ballSpeed.y *= -1;
                score += 10;
                
                // 10% 概率掉落道具
                if (GetRandomValue(0, 100) < 50) {
                    dropPowerUp = true;
                    dropPos = { brick.x + brick.width/2, brick.y + brick.height/2 };
                }
                return true;
            }
        }
    }
    return false;
}

bool BrickManager::AllCleared() {
    for (const auto& brick : bricks) {
        if (brick.active) return false;
    }
    return true;
}