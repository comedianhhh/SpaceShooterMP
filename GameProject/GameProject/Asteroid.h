#pragma once
#ifndef _AST_H_
#define _AST_H_

#include "GameCore.h"

class BoxCollider;

class Asteroid : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Asteroid, Component)

public:
    void Initialize() override;
    void Update() override;

    
    int health = 3;
    Entity* networkedEntity = nullptr;
    BoxCollider* collider = nullptr;

private:
    Vec2 direction = Vec2::Zero;
    float speed = 100.0f;
    float updateTimer=1.0f;

    void Move();
    void CheckCollision();
    void CheckBounds();
    void RPC(RakNet::BitStream& bitStream);
    void SendUpdate();
};

#endif // !_AST_H_
