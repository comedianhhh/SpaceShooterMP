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

    float frequency = 1.f;
    float lastSpawnTime = 1.0f;

    void SpawnAsteroid();
    Vec2 GetRandomPosition();
};

#endif // !_AST_SPAWNER_H_
