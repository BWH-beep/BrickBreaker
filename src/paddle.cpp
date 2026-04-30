#include "paddle.h"
#include <cstdio> 

Paddle::Paddle(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    size = { 120, 15 };
    speed = 500;
    Reset();
}

void Paddle::Update(float dt) {
    if (IsKeyDown(KEY_LEFT) && position.x - size.x/2 >= 0) {
        position.x -= speed * dt;
    }
    if (IsKeyDown(KEY_RIGHT) && position.x + size.x/2 <= screenWidth) {
        position.x += speed * dt;
    }
}

/*void Paddle::Draw() {
    DrawRectangleV(position, size, BLUE);
}*/
void Paddle::Draw() {
    Color baseColor = (Color){ 100, 180, 255, 255 };
    float x = position.x - size.x / 2;
    float y = position.y - size.y / 2;
    
    // 主挡板
    DrawRectangleRounded(
        (Rectangle){x, y, size.x, size.y},
        0.4f, 12, Fade(baseColor, 0.8f)
    );
    
    // 顶部高光
    DrawRectangleRounded(
        (Rectangle){x + 3, y + 1, size.x - 6, size.y * 0.4f},
        0.3f, 12, Fade(WHITE, 0.3f)
    );
    
    // 底部暗部
    DrawRectangleRounded(
        (Rectangle){x + 3, y + size.y * 0.6f, size.x - 6, size.y * 0.35f},
        0.3f, 12, Fade(BLACK, 0.1f)
    );
    
    // 边框
    DrawRectangleLines(x, y, size.x, size.y, Fade(WHITE, 0.5f));
}
void Paddle::Reset() {
    position = { screenWidth/2, screenHeight - 50 };
}

Vector2 Paddle::GetPosition() {
    return position;
}

Vector2 Paddle::GetSize() {
    return size;
}

Rectangle Paddle::GetRect() {
    return { position.x - size.x/2, position.y - size.y/2, size.x, size.y };
}
void Paddle::SetWidth(float newWidth) {
    size.x = newWidth;
}