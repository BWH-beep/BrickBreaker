#include "game.h"
#include <cmath>
#include <algorithm> 

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
    config = Config::load("config.json");  // 加载配置
    steps = config.steps;                   // 使用配置的步数
    InitWindow(screenWidth, screenHeight, "打砖块");
    LoadFont();
    powerUpTexture = LoadTexture("assets/images/powerup.png");
    // 初始化计时器
    for (int i = 0; i < 5; i++) {
    powerUpTimer[i] = 0.0f;
}
    originalPaddleWidth = paddle.GetSize().x;

// 初始化球（改成用 vector）
    balls.clear();
    balls.push_back(Ball());  // 添加一个球
    Reset();
    background = LoadTexture("assets/images/t04bc12585fc9f34567(1).png");
}

Game::~Game() {
    UnloadFont(chineseFont);
    UnloadTexture(background);
    UnloadTexture(powerUpTexture);
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
    // 清空道具
    powerUps.clear();
    for (int i = 0; i < 5; i++) {
        powerUpTimer[i] = 0.0f;
    }
    
    // 重置球（多个球）
    balls.clear();
    balls.push_back(Ball());
    balls[0].Reset();
    
    paddle.Reset();
    bricks.Reset();
    paddle.SetWidth(originalPaddleWidth);  // 恢复板子宽度
    
    state = GameState::WAITING;
    score = 0;
    lives = config.initialLives;
}

void Game::ProcessInput() {
    switch (state) {
    case GameState::WAITING:
        if (IsKeyPressed(KEY_SPACE)) {
            state = GameState::PLAYING;
            for (auto& b : balls) {
                b.Start(config.ballSpeedX, config.ballSpeedY);
            }
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
    bricks.UpdateParticles(dt);
    // 更新闪光特效
    if (flash.life > 0) {
        flash.life -= dt;
    }
    
    // 更新漂浮文字
    for (int i = floatingTexts.size() - 1; i >= 0; i--) {
        floatingTexts[i].pos.y -= 50 * dt;
        floatingTexts[i].life -= dt;
        if (floatingTexts[i].life <= 0) {
            floatingTexts.erase(floatingTexts.begin() + i);
        }
    }
    
    // 更新特效粒子
    for (int i = effectParticles.size() - 1; i >= 0; i--) {
        effectParticles[i].pos.x += effectParticles[i].vel.x * dt;
        effectParticles[i].pos.y += effectParticles[i].vel.y * dt;
        effectParticles[i].life -= dt;
        if (effectParticles[i].life <= 0) {
            effectParticles.erase(effectParticles.begin() + i);
        }
    }
    
    paddle.Update(dt);
    
    // 更新道具系统
    UpdatePowerUps(dt);
    
    if (state == GameState::PLAYING) {
        float stepDt = dt / steps;
        
        for (auto& ball : balls) {
            for (int step = 0; step < steps; step++) {
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
                
                // 板子碰撞
                Rectangle paddleRect = paddle.GetRect();
                Vector2 currentPos = ball.GetPosition();
                Vector2 hitPoint;
                
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
                bool dropPowerUp = false;
                bool hitEvil = false;
                Vector2 dropPos;
                
                bool hit = bricks.CheckCollision(ball.GetPosition(), ballRadius, ballSpeed, score, dropPowerUp, dropPos, hitEvil);
                
                if (hitEvil) {
                    state = GameState::GAMEOVER;
                    ball.Stop();
                    return;
                }
                if (hit) {
                    ball.SetSpeed(ballSpeed);
                }
                if (dropPowerUp) {
                    SpawnPowerUp(dropPos, GetRandomValue(0, 4));
                }
            }
        }
        
        // 检查球掉底
        for (int i = balls.size() - 1; i >= 0; i--) {
            if (balls[i].IsOutOfScreen(screenHeight)) {
                balls.erase(balls.begin() + i);
            }
        }
        
        if (balls.empty()) {
            lives--;
            if (lives <= 0) {
                state = GameState::GAMEOVER;
            } else {
                balls.clear();
                balls.push_back(Ball());
                balls[0].Reset();
                paddle.Reset();
                state = GameState::WAITING;
            }
        }
        
        if (bricks.AllCleared()) {
            state = GameState::WIN;
        }
    }
}
void Game::DrawChineseText(const char* text, int x, int y, int fontSize, Color color) {
    DrawTextEx(chineseFont, text, (Vector2){ (float)x, (float)y }, fontSize, 2, color);
}

void Game::Draw() {
    BeginDrawing();
    
    // 背景铺满屏幕
    float scaleX = (float)screenWidth / background.width;
    float scaleY = (float)screenHeight / background.height;
    float scale = (scaleX > scaleY) ? scaleX : scaleY;
    float drawX = (screenWidth - background.width * scale) / 2;
    float drawY = (screenHeight - background.height * scale) / 2;
    DrawTextureEx(background, (Vector2){drawX, drawY}, 0, scale, WHITE);
    
    bricks.Draw();
    
    for (auto& b : balls) {
        b.Draw();
    }
    
    paddle.Draw();
    DrawPowerUps();
    
    char scoreText[50];
    sprintf(scoreText, "得分: %d", score);
    DrawChineseText(scoreText, 10, 10, 28, WHITE);
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
    
    // ========== 特效绘制 ==========
    // 全屏闪光
    if (flash.life > 0) {
        float alpha = flash.intensity * flash.life * 2;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, alpha));
    }
    
    // 漂浮文字
    for (const auto& ft : floatingTexts) {
        int fontSize = 28 * (1.0f + (1.0f - ft.life));
        DrawTextEx(chineseFont, ft.text, ft.pos, fontSize, 2, Fade(ft.color, ft.life));
    }
    
    // 特效粒子
    for (const auto& p : effectParticles) {
        float size = p.size * (1.0f + (1.0f - p.life));
        DrawCircleV(p.pos, size, Fade(p.color, p.life * 1.5f));
    }
    // ===========================
    
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
void Game::SpawnPowerUp(Vector2 pos, int type) {
    PowerUp p;
    p.position = pos;
    p.active = true;
    p.type = type;
    p.speedY = 100.0f;  // 下落速度（像素/秒）
    powerUps.push_back(p);
}
void Game::ApplyPowerUp(int type) {
    Vector2 center = paddle.GetPosition();
    center.y -= 20;
    
    switch (type) {
        case 0: // 加长板子
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 2.0f);
            powerUpTimer[0] = 6.0f;
            // 金色光环特效
            for (int i = 0; i < 80; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 600);
                p.vel = { cos(angle) * speed, sin(angle) * speed };
                p.life = 0.8f;
                p.size = 4;
                p.color = GOLD;
                effectParticles.push_back(p);
            }
            flash.intensity = 0.8f;
            flash.life = 0.3f;
            break;
            
        case 1: // 缩短板子（敌方效果）
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 0.5f);
            powerUpTimer[1] = 5.0f;
            // 暗紫色特效
            for (int i = 0; i < 60; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(150, 450);
                p.vel = { cos(angle) * speed, sin(angle) * speed };
                p.life = 0.6f;
                p.size = 5;
                p.color = DARKPURPLE;
                effectParticles.push_back(p);
            }
            break;
            
        case 2: // 多球（终极爆炸）
            {
                // 超大爆炸光环
                for (int r = 0; r < 3; r++) {
                    for (int i = 0; i < 120; i++) {
                        EffectParticle p;
                        p.pos = center;
                        float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                        float speed = (float)GetRandomValue(300, 800);
                        p.vel = { cos(angle) * speed, sin(angle) * speed };
                        p.life = 0.7f - r * 0.1f;
                        p.size = (r == 0) ? 6 : 3;
                        if (r == 0) p.color = ORANGE;
                        else if (r == 1) p.color = YELLOW;
                        else p.color = RED;
                        effectParticles.push_back(p);
                    }
                }
                
                // 产生8-12个新球
                int newBallsCount = GetRandomValue(8, 12);
                for (int i = 0; i < newBallsCount; i++) {
                    Ball newBall = balls[0];
                    float angle = (float)i * (360.0f / newBallsCount) * DEG2RAD;
                    float radius = 60.0f;
                    Vector2 offset = { cos(angle) * radius, sin(angle) * radius };
                    newBall.SetPosition({ center.x + offset.x, center.y + offset.y });
                    Vector2 speed = { cos(angle) * 350, sin(angle) * 350 };
                    newBall.SetSpeed(speed);
                    newBall.Start(speed.x, speed.y);
                    balls.push_back(newBall);
                }
                
                // 屏幕强烈震动
                shakeTime = 0.5f;
                flash.intensity = 1.0f;
                flash.life = 0.4f;
                
                // 加分文字
                FloatingText ft;
                ft.pos = center;
                sprintf(ft.text, "+%d BALLS!", newBallsCount);
                ft.life = 1.0f;
                ft.color = YELLOW;
                floatingTexts.push_back(ft);
                break;
            }
            
        case 3: // 慢速球
            for (auto& b : balls) {
                Vector2 spd = b.GetSpeed();
                spd.x *= 0.3f;
                spd.y *= 0.3f;
                b.SetSpeed(spd);
            }
            powerUpTimer[3] = 5.0f;
            // 蓝色冰冻特效
            for (int i = 0; i < 50; i++) {
                EffectParticle p;
                p.pos = center;
                p.vel = { (float)GetRandomValue(-200, 200), (float)GetRandomValue(-300, 100) };
                p.life = 0.5f;
                p.size = 4;
                p.color = SKYBLUE;
                effectParticles.push_back(p);
            }
            break;
            
        case 4: // 加分
            score += 100;
            // 金币爆炸特效
            for (int i = 0; i < 100; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 500);
                p.vel = { cos(angle) * speed, sin(angle) * speed };
                p.life = 0.6f;
                p.size = (i % 2 == 0) ? 5 : 3;
                p.color = GOLD;
                effectParticles.push_back(p);
            }
            FloatingText ft;
            ft.pos = center;
            sprintf(ft.text, "+100");
            ft.life = 1.0f;
            ft.color = GOLD;
            floatingTexts.push_back(ft);
            break;
    }
}
void Game::UpdatePowerUps(float dt) {
    // 1. 更新计时器
    for (int i = 0; i < 5; i++) {
        if (powerUpTimer[i] > 0) {
            powerUpTimer[i] -= dt;
            if (powerUpTimer[i] <= 0) {
                // 时间到，恢复效果
                if (i == 0 || i == 1) {  // 加长/缩短板子
                    paddle.SetWidth(originalPaddleWidth);
                }
                if (i == 3) {  // 慢速球
                    for (auto& b : balls) {
                        b.SetSpeed(b.GetOriginalSpeed());
                    }
                }
            }
        }
    }
    
    // 2. 更新道具位置和碰撞
    for (auto& p : powerUps) {
        if (!p.active) continue;
        
        p.position.y += p.speedY * dt;  // 向下移动
        
        // 超出屏幕底部
        if (p.position.y > screenHeight) {
            p.active = false;
            continue;
        }
        
        // 检查是否被板子接住
        Rectangle paddleRect = paddle.GetRect();
        if (CheckCollisionCircleRec(p.position, 20, paddleRect)) {
            ApplyPowerUp(p.type);
            p.active = false;
        }
    }
    
    // 3. 删除失效道具
    powerUps.erase(remove_if(powerUps.begin(), powerUps.end(),
        [](const PowerUp& p) { return !p.active; }), powerUps.end());
}
void Game::DrawPowerUps() {
    for (const auto& p : powerUps) {
        if (p.active) {
            DrawTextureEx(powerUpTexture, p.position, 0, 0.01f, WHITE); 
        }
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