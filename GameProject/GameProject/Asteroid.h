#pragma once
#ifndef _AST_H_
#define _AST_H_

#include "GameCore.h"

class CircleCollider;

class Asteroid : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

public:
    void Initialize() override;
    void Update() override;

    
    Health* health = nullptr;
    Entity* networkedEntity = nullptr;
    CircleCollider* collider = nullptr;

private:
    Vec2 direction = Vec2::Zero;
    float speed = 5.0f;

    void Move();
    void CheckCollision();
    void CheckBounds();
};

#endif // !_AST_H_
