#include "game.h"
#include <cmath>

// 线段与矩形碰撞检测函数
bool LineRectCollision(Vector2 start, Vector2 end, Rectangle rect, Vector2& hitPoint) {
    // 检查矩形的四条边
    Vector2 edges[4][2] = {
        {{rect.x, rect.y}, {rect.x + rect.width, rect.y}},
        {{rect.x + rect.width, rect.y}, {rect.x + rect.width, rect.y + rect.height}},
        {{rect.x + rect.width, rect.y + rect.height}, {rect.x, rect.y + rect.height}},
        {{rect.x, rect.y + rect.height}, {rect.x, rect.y}}
    };
    
    for (int i = 0; i < 4; i++) {
        Vector2 p1 = edges[i][0];
        Vector2 p2 = edges[i][1];
        
        float denom = (p2.y - p1.y) * (end.x - start.x) - (p2.x - p1.x) * (end.y - start.y);
        if (fabs(denom) < 0.0001f) continue;
        
        float ua = ((p2.x - p1.x) * (start.y - p1.y) - (p2.y - p1.y) * (start.x - p1.x)) / denom;
        float ub = ((end.x - start.x) * (start.y - p1.y) - (end.y - start.y) * (start.x - p1.x)) / denom;
        
        if (ua >= 0 && ua <= 1 && ub >= 0 && ub <= 1) {
            hitPoint.x = start.x + ua * (end.x - start.x);
            hitPoint.y = start.y + ua * (end.y - start.y);
            return true;
        }
    }
    return false;
}

Game::Game(int width, int height) 
    : screenWidth(width), screenHeight(height), 
      paddle(width, height), bricks(width) {
    InitWindow(screenWidth, screenHeight, "打砖块");
    LoadFont();
    Reset();
}

Game::~Game() {
    UnloadFont(chineseFont);
    CloseWindow();
}

void Game::LoadFont() {
    int codepoints[] = { 
        0x5F97, 0x5206, 0x6309, 0x7A7A, 0x683C, 0x952E, 0x5F00, 0x59CB, 0x6E38, 0x620F,
        0x5DE6, 0x53F3, 0x79FB, 0x52A8, 0x677F, 0x5B50, 0x7ED3, 0x675F, 0x6700, 0x7EC8,
        0x91CD, 0x65B0, 0x80DC, 0x5229, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
    };
    int codepointCount = sizeof(codepoints) / sizeof(codepoints[0]);
    chineseFont = LoadFontEx("/mnt/c/Windows/Fonts/simhei.ttf", 48, codepoints, codepointCount);
}

void Game::Reset() {
    ball.Reset();
    paddle.Reset();
    bricks.Reset();
    state = GameState::WAITING;
    score = 0;
    lives=3;
}

void Game::ProcessInput() {
    switch (state) {
    case GameState::WAITING:
        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::PLAYING;
            ball.Start();
        }
        break;
        
    case GameState::GAMEOVER:
    case GameState::WIN:
        if (IsKeyPressed(KEY_SPACE)) {
            Reset();
        }
        break;
        
    default:
        break;
}
}

void Game::Update(float dt) {
    paddle.Update(dt);
    
    if (state == GameState::PLAYING){
        float stepDt = dt / STEPS;
        
        for (int step = 0; step < STEPS; step++) {
            Vector2 oldPos = ball.GetPosition();
            ball.Update(stepDt);
            
            Vector2 ballPos = ball.GetPosition();
            float ballRadius = ball.GetRadius();
            
            // 墙壁碰撞
            if (ballPos.x - ballRadius < 0) {
                ball.SetPositionX(ballRadius);
                ball.BounceX();
            }
            if (ballPos.x + ballRadius > screenWidth) {
                ball.SetPositionX(screenWidth - ballRadius);
                ball.BounceX();
            }
            if (ballPos.y - ballRadius < 0) {
                ball.SetPositionY(ballRadius);
                ball.BounceY();
            }
            
            // 板子碰撞 - 使用线段检测
            Rectangle paddleRect = paddle.GetRect();
            Vector2 currentPos = ball.GetPosition();
            Vector2 hitPoint;
            
            // 扩展矩形（考虑球的半径）
            Rectangle expandedRect = {
                paddleRect.x - ballRadius,
                paddleRect.y - ballRadius,
                paddleRect.width + ballRadius * 2,
                paddleRect.height + ballRadius * 2
            };
            
            if (LineRectCollision(oldPos, currentPos, expandedRect, hitPoint)) {
                if (ball.GetSpeed().y > 0) {
                    float hitPos = (hitPoint.x - paddle.GetPosition().x) / (paddle.GetSize().x / 2);
                    if (hitPos > 1) hitPos = 1;
                    if (hitPos < -1) hitPos = -1;
                    
                    ball.BounceY();
                    ball.AddSpeedX(hitPos * 150);
                    ball.ClampSpeed(500);
                    ball.SetPositionY(paddleRect.y - ballRadius);
                }
            }
            
            // 砖块碰撞
            Vector2 ballSpeed = ball.GetSpeed();
            bool hit;
            int loopCount = 0;
            do {
                hit = bricks.CheckCollision(ball.GetPosition(), ballRadius, ballSpeed, score);
                loopCount++;
                if (loopCount > 10) break;
            } while (hit);
            ball.SetSpeed(ballSpeed);
        }
        
        if (ball.IsOutOfScreen(screenHeight)) {
            lives--;
            if (lives <= 0) {
                state = GameState::GAMEOVER;
                ball.Stop();
            } else {
                ball.Reset();
                paddle.Reset();
                state = GameState::WAITING;
                ball.Stop();
    }
        }
        
        if (bricks.AllCleared()) {
             state = GameState::WIN;
            ball.Stop();
        }
    }
}

void Game::DrawChineseText(const char* text, int x, int y, int fontSize, Color color) {
    DrawTextEx(chineseFont, text, (Vector2){ (float)x, (float)y }, fontSize, 2, color);
}

void Game::Draw() {
    BeginDrawing();
    ClearBackground(BLACK);
    
    bricks.Draw();
    ball.Draw();
    paddle.Draw();
    
    char scoreText[50];
    sprintf(scoreText, "得分: %d", score);
    DrawChineseText(scoreText, 10, 10, 28, WHITE);
    // 绘制血条（心形）
    DrawHearts();
    
    if (state == GameState::WAITING) {
        DrawChineseText("按空格键开始游戏", screenWidth/2 - 130, screenHeight/2, 28, YELLOW);
        DrawChineseText("左右键移动板子", screenWidth/2 - 100, screenHeight/2 + 50, 24, WHITE);
    }
    
    if (state == GameState::GAMEOVER){
        DrawChineseText("游戏结束", screenWidth/2 - 70, screenHeight/2 - 60, 48, RED);
        char finalScore[50];
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 10, 28, WHITE);
        DrawChineseText("按空格键重新开始", screenWidth/2 - 110, screenHeight/2 + 40, 24, WHITE);
    }
    
    if (state == GameState::WIN) {
        DrawChineseText("胜利", screenWidth/2 - 40, screenHeight/2 - 60, 56, GREEN);
        char finalScore[50];
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 10, 28, WHITE);
        DrawChineseText("按空格键重新开始", screenWidth/2 - 110, screenHeight/2 + 40, 24, WHITE);
    }
    
    EndDrawing();
}

void Game::Run() {
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        ProcessInput();
        Update(dt);
        Draw();
    }
}
void Game::DrawHearts() {
    int startX = screenWidth - 70;
    int startY = 20;
    int spacing = 50;
    
    for (int i = 0; i < lives; i++) {
        int x = startX - i * spacing;
        
        // 标准爱心形状（24x24 像素点阵）
        int heartPixels[24][24] = {
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,0,0,0,0,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,0,0,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
            {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
            {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
            {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
        };
        
        for (int row = 0; row < 24; row++) {
            for (int col = 0; col < 24; col++) {
                if (heartPixels[row][col] == 1) {
                    DrawPixel(x + col - 12, startY + row - 12, RED);
                }
            }
        }
    }
}