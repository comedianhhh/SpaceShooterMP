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
    void SetID(uint64_t ID) { PlayerID = ID; }

	void SerializeCreate(RakNet::BitStream& bitStream) const override;
	void DeserializeCreate(RakNet::BitStream& bitStream) override;
private:


    BoxCollider* collider = nullptr;
    STRCODE game_over_scene = -1;
    Health* health = nullptr;
    Entity* networkedEntity = nullptr;
    uint64_t PlayerID;


private:
    void RPC(RakNet::BitStream& bitStream);

private:
    void Fire();
    
    void CheckCollision();
    float cooldownTimer = 0.5f;
    float lastFireTime = 0.0f;

};

#endif // PLAYER_H


