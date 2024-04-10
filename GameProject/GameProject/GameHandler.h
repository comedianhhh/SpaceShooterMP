#pragma once
#ifndef _GAME_HANDLER_H_
#define _GAME_HANDLER_H_

#include "Component.h"
class GameHandler : public Component
{
	DECLARE_SINGLETON(GameHandler)

private:

	bool bGameStarted = false;

	int NoPlayers = 0;



public:

	void Update() override;

	bool IsGameStarted() { return bGameStarted; }

	void StartGame() { bGameStarted = true; }
};

#endif // !_GAME_HANDLER_H_
