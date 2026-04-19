#ifndef GAME_OBJECT_H
#define GAME_OBJECT_H

#include "raylib.h"

// ========== 1. GameObject 基类 ==========
class GameObject {
protected:
    Vector2 position;
    
public:
    GameObject() : position({0, 0}) {}
    GameObject(Vector2 pos) : position(pos) {}
    
    Vector2 GetPosition() const { return position; }
    void SetPosition(Vector2 pos) { position = pos; }
};

// ========== 2. PhysicalObject 类 ==========
class PhysicalObject : virtual public GameObject {
protected:
    Vector2 velocity;
    float radius;
    
public:
    PhysicalObject() : velocity({0, 0}), radius(0) {}
    PhysicalObject(Vector2 pos, Vector2 vel, float r) 
        : GameObject(pos), velocity(vel), radius(r) {}
    
    Vector2 GetVelocity() const { return velocity; }
    void SetVelocity(Vector2 vel) { velocity = vel; }
    float GetRadius() const { return radius; }
    void SetRadius(float r) { radius = r; }
};

// ========== 3. VisualObject 类 ==========
class VisualObject : virtual public GameObject {
protected:
    Color color;
    bool visible;
    
public:
    VisualObject() : color(WHITE), visible(true) {}
    VisualObject(Vector2 pos, Color c, bool vis) 
        : GameObject(pos), color(c), visible(vis) {}
    
    Color GetColor() const { return color; }
    void SetColor(Color c) { color = c; }
    bool IsVisible() const { return visible; }
    void SetVisible(bool vis) { visible = vis; }
};

#endif