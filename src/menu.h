#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum class GameMode {
    SINGLE_PLAYER,
    HOST,
    CLIENT
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
    
    bool hasSave;
    bool loadSave;
    int savedLevel;
    int savedScore;
    int savedLives;
    
    bool inMainMenu;
    int mainMenuChoice;
    
    void Init() {
        mode = GameMode::SINGLE_PLAYER;
        selectedMode = 0;
        selectedLevel = 0;
        selectedDiff = 1;
        inMenu = true;
        isChinese = false;
        choosingLevel = true;
        hasSave = false;
        loadSave = false;
        savedLevel = 0;
        savedScore = 0;
        savedLives = 3;
        inMainMenu = true;
        mainMenuChoice = 0;
        isChinese = chinese;
    }
    
    void Update() {
        if (inMainMenu && hasSave) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                mainMenuChoice = (mainMenuChoice - 1 + 2) % 2;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                mainMenuChoice = (mainMenuChoice + 1) % 2;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (mainMenuChoice == 0) {
                    hasSave = false;
                    loadSave = false;
                    inMainMenu = false;
                } else {
                    loadSave = true;
                    inMenu = false;
                }
            }
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
                selectedMode = (selectedMode - 1 + 3) % 3;
            }
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
                selectedMode = (selectedMode + 1) % 3;
            }
            mode = (GameMode)selectedMode;
            if (IsKeyPressed(KEY_L)) isChinese = !isChinese;
            return;
        }
        
        inMainMenu = false;
        
        if (choosingLevel) {
            if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
                selectedLevel = (selectedLevel - 1 + 4) % 4;
            }
            if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
                selectedLevel = (selectedLevel + 1) % 4;
            }
            if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
                if (selectedLevel == 3) {
                    inMenu = false;  // 自定义关卡直接开始
                } else {
                    choosingLevel = false;
                }
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
        static Texture2D menuBg = LoadTexture("assets/images/menu_bg.png");
    DrawTexturePro(menuBg, (Rectangle){0, 0, (float)menuBg.width, (float)menuBg.height}, 
               (Rectangle){0, 0, 800, 600}, (Vector2){0, 0}, 0, WHITE);
    DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.7f));
        
        const char* title = isChinese ? "弹球风暴" : "BRICK BREAKER";
        Vector2 titleSize = MeasureTextEx(font, title, 70, 2);
        DrawTextEx(font, title, (Vector2){400 - titleSize.x/2, 30}, 70, 2, YELLOW);
        
        DrawTextEx(font, isChinese ? "游戏模式:" : "Game Mode:", (Vector2){60, 120}, 28, 2, WHITE);
        const char* modesCN[] = { "单人", "主机", "客户端" };
        const char* modesEN[] = { "Single", "Host", "Client" };
        const char** modes = isChinese ? modesCN : modesEN;
        for (int i = 0; i < 3; i++) {
            Color c = (selectedMode == i) ? YELLOW : (Color){180,180,180,255};
            DrawTextEx(font, modes[i], (Vector2){300 + i * 140, 120}, 26, 2, c);
        }
        
        DrawLine(60, 170, 740, 170, Fade(GRAY, 0.4f));
        
        if (inMainMenu && hasSave) {
            DrawTextEx(font, isChinese ? "检测到存档" : "Save File Found", (Vector2){220, 200}, 32, 2, WHITE);
            char buf[64];
            sprintf(buf, isChinese ? "关卡: %d  分数: %d  生命: %d" : "Level: %d  Score: %d  Lives: %d", savedLevel + 1, savedScore, savedLives);
            DrawTextEx(font, buf, (Vector2){220, 250}, 22, 2, GRAY);
            
            DrawTextEx(font, isChinese ? "新游戏" : "New Game", (Vector2){300, 320}, 30, 2, mainMenuChoice == 0 ? YELLOW : (Color){200,200,200,255});
            DrawTextEx(font, isChinese ? "继续存档" : "Continue", (Vector2){300, 370}, 30, 2, mainMenuChoice == 1 ? YELLOW : (Color){200,200,200,255});
            
            DrawTextEx(font, isChinese ? "上下键选择  ENTER 确认" : "UP/DOWN to Select  ENTER to Confirm", (Vector2){230, 450}, 22, 2, GRAY);
            DrawTextEx(font, isChinese ? "<- -> 切换模式  按 L 切换语言" : "<- -> Change Mode  Press L for Language", (Vector2){200, 500}, 20, 2, GRAY);
            return;
        }
        
        if (choosingLevel) {
            DrawTextEx(font, isChinese ? "选择关卡:" : "Select Level:", (Vector2){250, 200}, 32, 2, WHITE);
            const char* levelsCN[] = { "第一关 - 爱心", "第二关 - 猫咪", "第三关 - 蝴蝶", "自定义关卡" };
            const char* levelsEN[] = { "Level 1 - Heart", "Level 2 - Cat", "Level 3 - Butterfly", "Custom Level" };
            const char** levels = isChinese ? levelsCN : levelsEN;
            for (int i = 0; i < 4; i++) {
                Color c = (selectedLevel == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, levels[i], (Vector2){250, 260 + i * 55}, 30, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 确认关卡" : "ENTER to confirm", (Vector2){300, 490}, 22, 2, GRAY);
        } else {
            DrawTextEx(font, isChinese ? "选择难度:" : "Select Difficulty:", (Vector2){250, 200}, 32, 2, WHITE);
            const char* diffsCN[] = { "简单", "普通", "困难" };
            const char* diffsEN[] = { "Easy", "Normal", "Hard" };
            const char** diffs = isChinese ? diffsCN : diffsEN;
            for (int i = 0; i < 3; i++) {
                Color c = (selectedDiff == i) ? YELLOW : (Color){200,200,200,255};
                DrawTextEx(font, diffs[i], (Vector2){320, 260 + i * 55}, 30, 2, c);
            }
            DrawTextEx(font, isChinese ? "ENTER 开始 | ESC 返回" : "ENTER Start | ESC Back", (Vector2){260, 450}, 22, 2, GRAY);
        }
        
        DrawTextEx(font, isChinese ? "语言: 中文 | 按 L" : "Lang: EN | Press L", (Vector2){40, 520}, 20, 2, GRAY);
        DrawTextEx(font, isChinese ? "<- -> 切换模式" : "<- -> Change Mode", (Vector2){200, 570}, 22, 2, GRAY);
    }
    
    int GetBrickRows() { return 5; }
};

#endif