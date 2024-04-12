#pragma once
#ifndef _GAME_HANDLER_H_
#define _GAME_HANDLER_H_

#include "Component.h"
class GameHandler : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(GameHandler,Component)

private:
	bool bGameStarted = false;
	bool bPlayerSpawned = false;
	void SpawnPlayer();
	float spawnDelay = 1.0f;
	Vec2 GenerateUniformPosition();
public:
	void Update() override;
	void Initialize() override;
	bool IsGameStarted() const { return bGameStarted; }
	void RPC(RakNet::BitStream& bitStream);

};

#endif // !_GAME_HANDLER_H_
