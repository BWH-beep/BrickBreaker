#include "game.h"

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
    gameStarted = false;
    gameOver = false;
    win = false;
    score = 0;
}

void Game::ProcessInput() {
    if (!gameStarted && !gameOver && !win && IsKeyPressed(KEY_SPACE)) {
        gameStarted = true;
        ball.Start();
    }
    
    if ((gameOver || win) && IsKeyPressed(KEY_SPACE)) {
        Reset();
    }
}

void Game::Update(float dt) {
    paddle.Update(dt);
    
    if (!gameOver && !win && gameStarted) {
        float stepDt = dt / STEPS;
        
        for (int step = 0; step < STEPS; step++) {
            ball.Update(stepDt);
            
            // 墙壁碰撞
            Vector2 ballPos = ball.GetPosition();
            float ballRadius = ball.GetRadius();
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
            
            // 板子碰撞
            Rectangle paddleRect = paddle.GetRect();
            if (CheckCollisionCircleRec(ball.GetPosition(), ball.GetRadius(), paddleRect)) {
                if (ball.GetSpeed().y > 0) {
                    float hitPos = (ball.GetPosition().x - paddle.GetPosition().x) / (paddle.GetSize().x / 2);
                    if (hitPos > 1) hitPos = 1;
                    if (hitPos < -1) hitPos = -1;
                    
                    ball.BounceY();
                    ball.AddSpeedX(hitPos * 150);
                    ball.ClampSpeed(400);
                    ball.SetPositionY(paddle.GetRect().y - ball.GetRadius());
                }
            }
            
            // 砖块碰撞
            Vector2 ballSpeed = ball.GetSpeed();
            bricks.CheckCollision(ball.GetPosition(), ball.GetRadius(), ballSpeed, score);
        }
        
        if (ball.IsOutOfScreen(screenHeight)) {
            gameOver = true;
            ball.Stop();
        }
        
        if (bricks.AllCleared()) {
            win = true;
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
    
    if (!gameStarted && !gameOver && !win) {
        DrawChineseText("按空格键开始游戏", screenWidth/2 - 130, screenHeight/2, 28, YELLOW);
        DrawChineseText("左右键移动板子", screenWidth/2 - 100, screenHeight/2 + 50, 24, WHITE);
    }
    
    if (gameOver) {
        DrawChineseText("游戏结束", screenWidth/2 - 70, screenHeight/2 - 60, 48, RED);
        char finalScore[50];
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 10, 28, WHITE);
        DrawChineseText("按空格键重新开始", screenWidth/2 - 110, screenHeight/2 + 40, 24, WHITE);
    }
    
    if (win && !gameOver) {
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