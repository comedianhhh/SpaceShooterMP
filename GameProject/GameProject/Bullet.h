#pragma once
#ifndef BULLET_H
#define BULLET_H

#include "GameCore.h"
#include "BoxCollider.h"


class Bullet :public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Bullet, Component)

public:
	void Initialize() override;
	void Update() override;

	void SetDirection(const Vec2& direction);
	void SetSpeed(float speed);

private:
	Vec2 direction = Vec2::Zero;
	float speed = 100.0f;

	BoxCollider* collider = nullptr;

	void CheckCollision();
	void CheckBounds();
	void Move();

	void RPC(RakNet::BitStream& bitStream);
};

#endif