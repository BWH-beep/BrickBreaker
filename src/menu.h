#ifndef MENU_H
#define MENU_H

#include "raylib.h"

enum class GameMode {
    SINGLE_PLAYER,
    TWO_PLAYER
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
            selectedMode = (selectedMode - 1 + 2) % 2;
        }
        if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_D)) {
            selectedMode = (selectedMode + 1) % 2;
        }
        
        mode = (selectedMode == 0) ? GameMode::SINGLE_PLAYER : GameMode::TWO_PLAYER;
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
        DrawRectangle(0, 0, 800, 600, Fade(BLACK, 0.85f));
        
        const char* title = "BRICK BREAKER";
        Vector2 titleSize = MeasureTextEx(font, title, 60, 2);
        DrawTextEx(font, title, (Vector2){400 - titleSize.x/2, 80}, 60, 2, YELLOW);
        
        DrawTextEx(font, "Game Mode:", (Vector2){250, 200}, 28, 2, WHITE);
        const char* modes[] = { "Single Player", "Two Player" };
        for (int i = 0; i < 2; i++) {
            Color color = (selectedMode == i) ? YELLOW : GRAY;
            DrawTextEx(font, modes[i], (Vector2){420, 200 + i * 40}, 24, 2, color);
        }
        
        DrawTextEx(font, "Difficulty:", (Vector2){250, 320}, 28, 2, WHITE);
        const char* diffs[] = { "Easy (3 rows)", "Normal (5 rows)", "Hard (7 rows)" };
        for (int i = 0; i < 3; i++) {
            Color color = (selectedDiff == i) ? YELLOW : GRAY;
            DrawTextEx(font, diffs[i], (Vector2){420, 320 + i * 40}, 24, 2, color);
        }
        
        DrawTextEx(font, "<- -> / A D : Change Mode", (Vector2){200, 500}, 20, 2, GRAY);
        DrawTextEx(font, "UP DOWN / W S : Change Difficulty", (Vector2){200, 530}, 20, 2, GRAY);
        DrawTextEx(font, "SPACE / ENTER : Start Game", (Vector2){200, 560}, 20, 2, GRAY);
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