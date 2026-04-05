#include "ball.h"

Ball::Ball() {
    radius = 8;
    Reset();
}

void Ball::Reset() {
    position = { 400, 400 };
    speed = { 0, 0 };
    moving = false;
}

void Ball::Update(float dt) {
    if (moving) {
        position.x += speed.x * dt;
        position.y += speed.y * dt;
    }
}

void Ball::Draw() {
    DrawCircleV(position, radius, YELLOW);
}

void Ball::Start() {
    moving = true;
    speed = { 200, -200 };
}

void Ball::Stop() {
    moving = false;
}

void Ball::BounceX() {
    speed.x *= -1;
}

void Ball::BounceY() {
    speed.y *= -1;
}

void Ball::SetPositionX(float x) {
    position.x = x;
}

void Ball::SetPositionY(float y) {
    position.y = y;
}

void Ball::AddSpeedX(float delta) {
    speed.x += delta;
}

void Ball::ClampSpeed(float maxSpeed) {
    if (speed.x > maxSpeed) speed.x = maxSpeed;
    if (speed.x < -maxSpeed) speed.x = -maxSpeed;
    if (speed.y > maxSpeed) speed.y = maxSpeed;
    if (speed.y < -maxSpeed) speed.y = -maxSpeed;
}

Vector2 Ball::GetPosition() {
    return position;
}

Vector2 Ball::GetSpeed() {
    return speed;
}

float Ball::GetRadius() {
    return radius;
}

bool Ball::IsMoving() {
    return moving;
}

bool Ball::IsOutOfScreen(int screenHeight) {
    return position.y + radius > screenHeight;
}