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
    Difficulty diff;
    int selectedMode;
    int selectedDiff;
    bool inMenu;
    
    void Init() {
        mode = GameMode::SINGLE_PLAYER;
        diff = Difficulty::NORMAL;
        selectedMode = 0;
        selectedDiff = 1;
        inMenu = true;
    }
    
    void Update() {
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)) {
            selectedDiff = (selectedDiff - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)) {
            selectedDiff = (selectedDiff + 1) % 3;
        }
        if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_A)) {
            selectedMode = (selectedMode - 1 + 3) % 3;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            selectedMode = (selectedMode + 1) % 3;
        }
        
        mode = (GameMode)selectedMode;
        switch (selectedDiff) {
            case 0: diff = Difficulty::EASY; break;
            case 1: diff = Difficulty::NORMAL; break;
            case 2: diff = Difficulty::HARD; break;
        }
        
        if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
            inMenu = false;
        }
    }
    
    void Draw(Font font) {
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.95f));
        
        const char* title = "BRICK BREAKER";
        Vector2 titleSize = MeasureTextEx(font, title, 60, 2);
        DrawTextEx(font, title, (Vector2){400 - titleSize.x/2, 80}, 60, 2, YELLOW);
        
        DrawTextEx(font, "Game Mode:", (Vector2){200, 200}, 28, 2, WHITE);
        const char* modes[] = { "Single Player", "Create Room (Host)", "Join Room (Client)" };
        for (int i = 0; i < 3; i++) {
            Color color = (selectedMode == i) ? YELLOW : (Color){200, 200, 200, 255};
            DrawTextEx(font, modes[i], (Vector2){380, 200 + i * 40}, 24, 2, color);
        }
        
        DrawTextEx(font, "Difficulty:", (Vector2){200, 360}, 28, 2, WHITE);
        const char* diffs[] = { "Level 1 (Easy)", "Level 2 (Normal)", "Level 3 (Hard)" };
        for (int i = 0; i < 3; i++) {
            Color color = (selectedDiff == i) ? YELLOW : (Color){200, 200, 255, 255};
            DrawTextEx(font, diffs[i], (Vector2){380, 360 + i * 40}, 24, 2, color);
        }
        
        DrawTextEx(font, "<- -> / A D : Change Mode", (Vector2){200, 520}, 20, 2, GRAY);
        DrawTextEx(font, "UP DOWN / W S : Change Difficulty", (Vector2){200, 550}, 20, 2, GRAY);
        DrawTextEx(font, "SPACE / ENTER : Start Game", (Vector2){200, 580}, 20, 2, GRAY);
    }
    
    int GetBrickRows() {
        switch (diff) {
            case Difficulty::EASY: return 3;
            case Difficulty::NORMAL: return 5;
            case Difficulty::HARD: return 7;
        }
        return 5;
    }
};

#endif