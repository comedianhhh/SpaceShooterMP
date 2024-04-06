#pragma once
#ifndef PLAYER_H
#define PLAYER_H

#include "GameCore.h"
#include "Health.h"

class BoxCollider;

class Player : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(Player, Component)

public:
    void Initialize() override;
    void Update() override;
    void Load(json::JSON&) override;
private:


    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;
    Health* health = nullptr;
    Entity* networkedEntity = nullptr;

private:
    void RPC(RakNet::BitStream& bitStream);

private:
    void Fire();
    
    void CheckCollision();
    float cooldownTimer = 0.5f;
    float lastFireTime = 0.0f;

};

#endif // PLAYER_H


