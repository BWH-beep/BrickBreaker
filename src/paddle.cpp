#include "paddle.h"

Paddle::Paddle(int width, int height) {
    screenWidth = width;
    screenHeight = height;
    size = { 120, 15 };
    speed = 500;
    Reset();
}

void Paddle::Update(float dt) {
    if (IsKeyDown(KEY_LEFT) && position.x - size.x/2 > 0) {
        position.x -= speed * dt;
    }
    if (IsKeyDown(KEY_RIGHT) && position.x + size.x/2 < screenWidth) {
        position.x += speed * dt;
    }
}

void Paddle::Draw() {
    DrawRectangleV(position, size, BLUE);
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