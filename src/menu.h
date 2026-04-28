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
    int selectedLevel;
    int selectedDiff;
    bool inMenu;
    bool isChinese;
    bool choosingLevel;
    
    void Init() {
        mode = GameMode::SINGLE_PLAYER;
        selectedMode = 0;
        selectedLevel = 0;
        selectedDiff = 1;
        inMenu = true;
        isChinese = false;  // 默认英文
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
                choosingLevel = false;
            }
        } else {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedDiff = (selectedDiff - 1 + 3) % 3;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedDiff = (selectedDiff + 1) % 3;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                inMenu = false;
            }
            if (IsKeyPressed(KEY_ESCAPE)) {
                choosingLevel = true;
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
        
        // 标题
        const char* title = "BRICK BREAKER";
        Vector2 titleSize = MeasureTextEx(font, title, 70, 2);
        DrawTextEx(font, title, (Vector2){400 - titleSize.x/2, 30}, 70, 2, YELLOW);
        
        // 模式选择
        DrawTextEx(font, isChinese ? "游戏模式:" : "Game Mode:", (Vector2){60, 130}, 30, 2, WHITE);
        const char* modesCN[] = { "单人", "主机", "客户端" };
        const char* modesEN[] = { "Single", "Host", "Client" };
        const char** modes = isChinese ? modesCN : modesEN;
        for (int i = 0; i < 3; i++) {
            Color c = (selectedMode == i) ? YELLOW : (Color){180,180,180,255};
            DrawTextEx(font, modes[i], (Vector2){300 + i * 140, 130}, 26, 2, c);
        }
        
        // 分割线
        DrawLine(60, 180, 740, 180, Fade(GRAY, 0.4f));
        
        if (choosingLevel) {
            DrawTextEx(font, isChinese ? "选择关卡:" : "Select Level:", (Vector2){250, 210}, 32, 2, WHITE);
            const char* levelsCN[] = { "第一关 - 爱心", "第二关 - 猫咪", "第三关 - 蝴蝶" };
            const char* levelsEN[] = { "Level 1 - Heart", "Level 2 - Cat", "Level 3 - Butterfly" };
            const char** levels = isChinese ? levelsCN : levelsEN;
            for (int i = 0; i < 3; i++) {
                Color c = (selectedLevel == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, levels[i], (Vector2){250, 270 + i * 55}, 30, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 确认关卡" : "ENTER to confirm", (Vector2){300, 460}, 22, 2, GRAY);
        } else {
            DrawTextEx(font, isChinese ? "选择难度:" : "Select Difficulty:", (Vector2){250, 210}, 32, 2, WHITE);
            const char* diffsCN[] = { "简单", "普通", "困难" };
            const char* diffsEN[] = { "Easy", "Normal", "Hard" };
            const char** diffs = isChinese ? diffsCN : diffsEN;
            for (int i = 0; i < 3; i++) {
                Color c = (selectedDiff == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, diffs[i], (Vector2){320, 270 + i * 55}, 30, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 开始 | ESC 返回" : "ENTER Start | ESC Back", (Vector2){260, 460}, 22, 2, GRAY);
        }
        
        // 语言
        DrawTextEx(font, isChinese ? "语言: 中文 | 按 L" : "Lang: EN | Press L", (Vector2){40, 520}, 20, 2, GRAY);
        DrawTextEx(font, isChinese ? "<- -> 切换模式" : "<- -> Change Mode", (Vector2){200, 570}, 22, 2, GRAY);
    }
    
    int GetBrickRows() { return 5; }
};

#endif