#include <cstring>
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
    config = Config::load("config.json");
    steps = config.steps;
    InitWindow(screenWidth, screenHeight, "打砖块");
    LoadFont();
    powerUpTexture = LoadTexture("assets/images/powerup.png");
    
    // 初始化计时器
    for (int i = 0; i < 6; i++) {
        powerUpTimer[i] = 0.0f;
    }
    
    originalPaddleWidth = paddle.GetSize().x;

    // 初始化球
    balls.clear();
    balls.push_back(Ball());
    Reset();
    background = LoadTexture("assets/images/t04bc12585fc9f34567(1).png");
    paused = false;
    pauseButton = { screenWidth - 50.0f, 10.0f, 40.0f, 40.0f };
    continueButton = { screenWidth/2 - 100.0f, screenHeight/2 - 30.0f, 200.0f, 50.0f };
    quitButton = { screenWidth/2 - 100.0f, screenHeight/2 + 40.0f, 200.0f, 50.0f };
}


Game::~Game() {
    UnloadFont(chineseFont);
    UnloadTexture(background);
    UnloadTexture(powerUpTexture);
    CloseWindow();
}

/*void Game::LoadFont() {
    int codepoints[] = { 
        0x5F97, 0x5206, 0x6309, 0x7A7A, 0x683C, 0x952E, 0x5F00, 0x59CB, 0x6E38, 0x620F,
        0x5DE6, 0x53F3, 0x79FB, 0x52A8, 0x677F, 0x5B50, 0x7ED3, 0x675F, 0x6700, 0x7EC8,
        0x91CD, 0x65B0, 0x80DC, 0x5229, 0x30, 0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39
    };
    int codepointCount = sizeof(codepoints) / sizeof(codepoints[0]);
    chineseFont = LoadFontEx("/mnt/c/Windows/Fonts/simhei.ttf", 48, codepoints, codepointCount);
}*/
void Game::LoadFont() {
    // 计算需要加载的字符数量
    int asciiCount = 127 - 32;
    int hanziCount = 0x9FFF - 0x4E00 + 1;
    int totalCount = asciiCount + hanziCount;
    
    // 动态分配数组
    int* codepoints = new int[totalCount];
    int index = 0;
    
    // 加载 ASCII 字符
    for (int i = 32; i < 127; i++) {
        codepoints[index++] = i;
    }
    
    // 加载常用汉字
    for (int i = 0x4E00; i <= 0x9FFF; i++) {
        codepoints[index++] = i;
    }
    
    chineseFont = LoadFontEx("/mnt/c/Windows/Fonts/simhei.ttf", 48, codepoints, totalCount);
    
    // 释放内存
    delete[] codepoints;
}
void Game::Reset() {
    // 清空道具
    powerUps.clear();
    for (int i = 0; i < 6; i++) {
        powerUpTimer[i] = 0.0f;
    }
    
    // 重置球
    balls.clear();
    balls.push_back(Ball());
    balls[0].Reset();
    
    paddle.Reset();
    bricks.Reset();
    
    // 强制重置板子宽度为原始宽度
    originalPaddleWidth = config.paddleWidth;  // 从配置读取原始宽度
    paddle.SetWidth(originalPaddleWidth);
    
    state = GameState::WAITING;
    score = 0;
    lives = config.initialLives;
}

void Game::ProcessInput() {
    // 检测暂停按钮点击（不管是否暂停都要能点）
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
        Vector2 mousePos = GetMousePosition();
        if (CheckCollisionPointRec(mousePos, pauseButton)) {
            paused = !paused;
        }
    }
    
    if (paused) return;  // 暂停时不处理游戏输入
    
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
    if (paused) {
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePos = GetMousePosition();
            
            // 继续按钮
            if (CheckCollisionPointRec(mousePos, continueButton)) {
                paused = false;
            }
            
            // 退出按钮 - 回到开始界面
            if (CheckCollisionPointRec(mousePos, quitButton)) {
                paused = false;
                state = GameState::WAITING;
                Reset();
            }
        }
        return;  // 暂停时不更新游戏逻辑
    }
    
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
                ball.RecordTrail();
                ball.RecordTrail(); 
                
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
                bool hitExplosive = false;
                Vector2 dropPos;
                
                bool hit = bricks.CheckCollision(ball.GetPosition(), ballRadius, ballSpeed, score, 
                                                  dropPowerUp, dropPos, hitEvil, hitExplosive);
                
                if (hitEvil) {
                    // 无敌状态下不受邪恶砖块影响
                    if (!ball.IsInvincible()) {
                        state = GameState::GAMEOVER;
                        ball.Stop();
                        return;
                    }
                }
                if (hit) {
                    ball.SetSpeed(ballSpeed);
                }
                if (dropPowerUp) {
                    // ========== 测试模式：100% 无敌道具 ==========
                    int type;
                    int rand = GetRandomValue(0, 100);
                    if (rand < 20) type = 5;       // 20% 无敌
                    else if (rand < 45) type = 2;  // 25% 分裂
                    else if (rand < 60) type = 0;  // 15% 加长
                    else if (rand < 75) type = 3;  // 15% 慢速
                    else if (rand < 88) type = 4;  // 13% 加分
                    else type = 1;                 // 12% 缩短
                    SpawnPowerUp(dropPos, type);
                }
            }
        }
        
        // 检查球掉底（无敌球会反弹）
        for (int i = balls.size() - 1; i >= 0; i--) {
            if (balls[i].GetPosition().y + balls[i].GetRadius() > screenHeight) {
                if (balls[i].IsInvincible()) {
                    // 无敌球底部反弹
                    balls[i].SetPositionY(screenHeight - balls[i].GetRadius());
                    balls[i].BounceY();
                    
                    // 反弹特效
                    flash.intensity = 0.5f;
                    flash.life = 0.2f;
                } else {
                    balls.erase(balls.begin() + i);
                }
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
    
    char scoreText[50] = {0};
    sprintf(scoreText, "得分: %d", score);
    DrawChineseText(scoreText, 10, 10, 28, WHITE);
    DrawHearts();
    
    if (state == GameState::WAITING) {
        DrawChineseText("按空格键开始游戏", screenWidth/2 - 130, screenHeight/2, 28, YELLOW);
        DrawChineseText("左右键移动板子", screenWidth/2 - 100, screenHeight/2 + 50, 24, WHITE);
    }
    
    if (state == GameState::GAMEOVER){
        DrawChineseText("游戏结束", screenWidth/2 - 70, screenHeight/2 - 60, 48, RED);
        char finalScore[50] = {0};
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 10, 28, WHITE);
        DrawChineseText("按空格键重新开始", screenWidth/2 - 110, screenHeight/2 + 40, 24, WHITE);
    }
    
    if (state == GameState::WIN) {
        DrawChineseText("胜利", screenWidth/2 - 40, screenHeight/2 - 60, 56, GREEN);
        char finalScore[50] = {0};
        sprintf(finalScore, "最终得分: %d", score);
        DrawChineseText(finalScore, screenWidth/2 - 80, screenHeight/2 - 10, 28, WHITE);
        DrawChineseText("按空格键重新开始", screenWidth/2 - 110, screenHeight/2 + 40, 24, WHITE);
    }
    
    // 特效绘制
    if (flash.life > 0) {
        float alpha = flash.intensity * flash.life * 2;
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(WHITE, alpha));
    }
    
    for (const auto& ft : floatingTexts) {
        int fontSize = 28 * (1.0f + (1.0f - ft.life));
        DrawTextEx(chineseFont, ft.text, ft.pos, fontSize, 2, Fade(ft.color, ft.life));
    }
    
    for (const auto& p : effectParticles) {
        float size = p.size * (1.0f + (1.0f - p.life));
        DrawCircleV(p.pos, size, Fade(p.color, p.life * 1.5f));
    }
        // ========== 绘制暂停按钮 ==========
    DrawRectangleRec(pauseButton, Fade(GRAY, 0.5f));
    DrawRectangle(pauseButton.x + 12, pauseButton.y + 10, 6, 20, WHITE);
    DrawRectangle(pauseButton.x + 22, pauseButton.y + 10, 6, 20, WHITE);
    
    // ========== 如果暂停，绘制菜单 ==========
    if (paused) {
        // 半透明遮罩
        DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.7f));
        
        // 标题
        DrawText("PAUSED", screenWidth/2 - 70, screenHeight/2 - 120, 40, WHITE);
        
        // 继续按钮
        DrawRectangleRec(continueButton, Fade(GREEN, 0.8f));
        DrawRectangleLinesEx(continueButton, 2, WHITE);
        DrawText("Continue", continueButton.x + 40, continueButton.y + 12, 24, WHITE);
        
        // 退出按钮
        DrawRectangleRec(quitButton, Fade(RED, 0.8f));
        DrawRectangleLinesEx(quitButton, 2, WHITE);
        DrawText("Quit", quitButton.x + 60, quitButton.y + 12, 24, WHITE);
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

void Game::SpawnPowerUp(Vector2 pos, int type) {
    PowerUp p;
    p.position = pos;
    p.active = true;
    p.type = type;
    p.speedY = 100.0f;
    powerUps.push_back(p);
}

void Game::ApplyPowerUp(int type) {
    Vector2 center = paddle.GetPosition();
    center.y -= 20;
    
    switch (type) {
        case 0: // 加长板子
        {
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 2.0f);
            powerUpTimer[0] = 6.0f;
            for (int i = 0; i < 80; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 600);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.8f;
                p.size = 4;
                p.color = GOLD;
                effectParticles.push_back(p);
            }
            flash.intensity = 0.8f;
            flash.life = 0.3f;
            break;
        }
            
        case 1: // 缩短板子
        {
            originalPaddleWidth = paddle.GetSize().x;
            paddle.SetWidth(originalPaddleWidth * 0.5f);
            powerUpTimer[1] = 5.0f;
            for (int i = 0; i < 60; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(150, 450);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.6f;
                p.size = 5;
                p.color = DARKPURPLE;
                effectParticles.push_back(p);
            }
            break;
        }
            
        case 2: // 多球
        {
            for (int r = 0; r < 3; r++) {
                for (int i = 0; i < 120; i++) {
                    EffectParticle p;
                    p.pos = center;
                    float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                    float speed = (float)GetRandomValue(300, 800);
                    p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                    p.life = 0.7f - r * 0.1f;
                    p.size = (r == 0) ? 6 : 3;
                    if (r == 0) p.color = ORANGE;
                    else if (r == 1) p.color = YELLOW;
                    else p.color = RED;
                    effectParticles.push_back(p);
                }
            }
            
            int newBallsCount = GetRandomValue(8, 12);
            for (int i = 0; i < newBallsCount; i++) {
                Ball newBall = balls[0];
                
                float angle;
                if (i % 2 == 0) {
                    angle = (30.0f + (float)(i/2) * (120.0f / (newBallsCount/2))) * DEG2RAD;
                } else {
                    angle = (210.0f + (float)(i/2) * (120.0f / (newBallsCount/2))) * DEG2RAD;
                }
                
                float radius = 60.0f;
                Vector2 offset = { (float)(cos(angle) * radius), (float)(sin(angle) * radius) };
                newBall.SetPosition({ center.x + offset.x, center.y + offset.y });
                
                float speedX = (float)(cos(angle) * 350);
                float speedY = (float)(sin(angle) * 350);
                
                float minVerticalSpeed = 200.0f;
                if (fabs(speedY) < minVerticalSpeed) {
                    speedY = (speedY >= 0) ? minVerticalSpeed : -minVerticalSpeed;
                }
                
                Vector2 speed = { speedX, speedY };
                newBall.SetSpeed(speed);
                newBall.Start(speed.x, speed.y);
                balls.push_back(newBall);
            }
            
            shakeTime = 0.5f;
            flash.intensity = 1.0f;
            flash.life = 0.4f;
            
            FloatingText ft2;
        memset(ft2.text, 0, sizeof(ft2.text));
            ft2.pos = center;
            sprintf(ft2.text, "+%d 个球!", newBallsCount);
            ft2.life = 1.0f;
            ft2.color = YELLOW;
            floatingTexts.push_back(ft2);
            break;
        }
            
        case 3: // 慢速球
        {
            for (auto& b : balls) {
                Vector2 spd = b.GetSpeed();
                spd.x *= 0.3f;
                spd.y *= 0.3f;
                b.SetSpeed(spd);
            }
            powerUpTimer[3] = 5.0f;
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
        }
            
        case 4: // 加分
        {
            score += 100;
            for (int i = 0; i < 100; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 500);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.6f;
                p.size = (i % 2 == 0) ? 5 : 3;
                p.color = GOLD;
                effectParticles.push_back(p);
            }
            FloatingText ft4;
        memset(ft4.text, 0, sizeof(ft4.text));
            ft4.pos = center;
            sprintf(ft4.text, "+100 分");
            ft4.life = 1.0f;
            ft4.color = GOLD;
            floatingTexts.push_back(ft4);
            break;
        }
            
        case 5: // 无敌球
        {
            for (auto& b : balls) {
                b.SetInvincible(true, 8.0f);
            }
            powerUpTimer[5] = 8.0f;
            
            for (int i = 0; i < 80; i++) {
                EffectParticle p;
                p.pos = center;
                float angle = (float)GetRandomValue(0, 360) * DEG2RAD;
                float speed = (float)GetRandomValue(200, 600);
                p.vel = { (float)(cos(angle) * speed), (float)(sin(angle) * speed) };
                p.life = 0.8f;
                p.size = 5;
                p.color = (Color){ 
                    (unsigned char)GetRandomValue(100, 255), 
                    (unsigned char)GetRandomValue(100, 255), 
                    (unsigned char)GetRandomValue(100, 255), 
                    255 
                };
                effectParticles.push_back(p);
            }
            
            flash.intensity = 1.0f;
            flash.life = 0.5f;
            
            FloatingText ft5;
        memset(ft5.text, 0, sizeof(ft5.text));
            ft5.pos = center;
            sprintf(ft5.text, "无敌!");
            ft5.life = 1.5f;
            ft5.color = (Color){ 255, 215, 0, 255 };
            floatingTexts.push_back(ft5);
            break;
        }
    }
}

void Game::UpdatePowerUps(float dt) {
    // 更新计时器
    for (int i = 0; i < 6; i++) {
        if (powerUpTimer[i] > 0) {
            powerUpTimer[i] -= dt;
            if (powerUpTimer[i] <= 0) {
                if (i == 0 || i == 1) {
                    paddle.SetWidth(originalPaddleWidth);
                }
                if (i == 3) {
                    for (auto& b : balls) {
                        b.SetSpeed(b.GetOriginalSpeed());
                    }
                }
                if (i == 5) {
                    for (auto& b : balls) {
                        b.SetInvincible(false, 0);
                    }
                }
            }
        }
    }
    
    // 更新每个球的无敌状态
    for (auto& b : balls) {
        b.UpdateInvincible(dt);
    }
    
    // 更新道具位置和碰撞
    for (auto& p : powerUps) {
        if (!p.active) continue;
        
        p.position.y += p.speedY * dt;
        
        if (p.position.y > screenHeight) {
            p.active = false;
            continue;
        }
        
        Rectangle paddleRect = paddle.GetRect();
        if (CheckCollisionCircleRec(p.position, 20, paddleRect)) {
            ApplyPowerUp(p.type);
            p.active = false;
        }
    }
    
    // 删除失效道具
    powerUps.erase(remove_if(powerUps.begin(), powerUps.end(),
        [](const PowerUp& p) { return !p.active; }), powerUps.end());
}

void Game::DrawPowerUps() {
    for (const auto& p : powerUps) {
        if (p.active) {
            if (p.type == 5) {
                // 无敌道具用星星绘制
                float time = GetTime();
                DrawCircleV(p.position, 15, Fade(YELLOW, 0.5f));
                DrawCircleV(p.position, 10, Fade(WHITE, 0.8f));
                DrawText("★", p.position.x - 10, p.position.y - 10, 22, YELLOW);
            } else {
                DrawTextureEx(powerUpTexture, p.position, 0, 0.01f, WHITE);
            }
        }
    }
}

void Game::DrawHearts() {
    int startX = screenWidth - 70;
    int startY = 20;
    int spacing = 50;
    
    for (int i = 0; i < lives; i++) {
        int x = startX - i * spacing;
        
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
