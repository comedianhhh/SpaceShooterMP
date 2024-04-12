#pragma once
#ifndef _AST_SPAWNER_H_
#define _AST_SPAWNER_H_

#include "Component.h"

class AsteroidSpawner : public Component
{
    DECLARE_DYNAMIC_DERIVED_CLASS(AsteroidSpawner, Component)

public:
    void Initialize() override;
    void Update() override;

private:

    float frequency = 5.f;
    float lastSpawnTime = 0.0f;

    void SpawnAsteroid();

    float GetRandom(float min, float max);

    void RPC(RakNet::BitStream& bitStream);
};

#endif // !_AST_SPAWNER_H_
