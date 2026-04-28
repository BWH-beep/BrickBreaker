#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum class GameMode {
    SINGLE_PLAYER,   // 单机模式
    HOST,            // 创建房间（主机）
    CLIENT           // 加入房间（客户端）
};

enum class Difficulty {
    EASY,
    NORMAL,
    HARD
};

struct Menu {
    GameMode mode;
    int selectedMode;
    int selectedLevel;    // 0=爱心, 1=猫咪, 2=蝴蝶
    int selectedDiff;     // 0=Easy, 1=Normal, 2=Hard
    bool inMenu;
    bool isChinese;
    bool choosingLevel;   // true=正在选关卡, false=正在选难度
    
    void Init() {
        mode = GameMode::SINGLE_PLAYER;
        selectedMode = 0;
        selectedLevel = 0;
        selectedDiff = 1;
        inMenu = true;
        isChinese = true;
        choosingLevel = true;
    }
    
    void Update() {
        if (choosingLevel) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedLevel = (selectedLevel - 1 + 3) % 3;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedLevel = (selectedLevel + 1) % 3;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                choosingLevel = false;  // 确认关卡，进入难度选择
            }
        } else {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedDiff = (selectedDiff - 1 + 3) % 3;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedDiff = (selectedDiff + 1) % 3;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                inMenu = false;  // 开始游戏
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                choosingLevel = true;  // 返回关卡选择
            }
        }
        
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            selectedMode = (selectedMode - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            selectedMode = (selectedMode + 1) % 3;
        }
        mode = (GameMode)selectedMode;
        
        if (IsKeyPressed(KEY_L)) {
            isChinese = !isChinese;
        }
    }
    
    void Draw(Font font) {
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.95f));
        
        const char* title = "BRICK BREAKER";
        Vector2 titleSize = MeasureTextEx(font, title, 60, 2);
        DrawTextEx(font, title, (Vector2){400 - titleSize.x/2, 20}, 60, 2, YELLOW);
        
        // 模式选择（始终显示）
        DrawTextEx(font, isChinese ? "游戏模式:" : "Game Mode:", (Vector2){50, 120}, 24, 2, WHITE);
        const char* modes[] = { "Single", "Host", "Client" };
        for (int i = 0; i < 3; i++) {
            Color c = (selectedMode == i) ? YELLOW : GRAY;
            DrawTextEx(font, modes[i], (Vector2){250 + i * 120, 120}, 20, 2, c);
        }
        
        if (choosingLevel) {
            DrawTextEx(font, isChinese ? "选择关卡:" : "Select Level:", (Vector2){200, 200}, 28, 2, WHITE);
            const char* levels[] = { "Level 1 - 爱心", "Level 2 - 猫咪", "Level 3 - 蝴蝶" };
            for (int i = 0; i < 3; i++) {
                Color c = (selectedLevel == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, levels[i], (Vector2){280, 260 + i * 50}, 28, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 确认关卡" : "ENTER to confirm", (Vector2){280, 440}, 20, 2, GRAY);
        } else {
            DrawTextEx(font, isChinese ? "选择难度:" : "Select Difficulty:", (Vector2){200, 200}, 28, 2, WHITE);
            const char* diffs[] = { "Easy", "Normal", "Hard" };
            for (int i = 0; i < 3; i++) {
                Color c = (selectedDiff == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, diffs[i], (Vector2){320, 260 + i * 50}, 28, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 开始 | ESC 返回选关卡" : "ENTER Start | ESC Back", (Vector2){220, 440}, 20, 2, GRAY);
        }
        
        DrawTextEx(font, isChinese ? "语言: 中文 | 按 L 切换" : "Lang: Chinese | Press L", (Vector2){40, 500}, 18, 2, GRAY);
        DrawTextEx(font, isChinese ? "<- -> 切换模式" : "<- -> Change Mode", (Vector2){200, 550}, 20, 2, GRAY);
    }
    
    int GetBrickRows() { return 5; }
};

#endif