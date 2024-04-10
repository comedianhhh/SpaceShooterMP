#include "GameCore.h"
#include "GameHandler.h"
#include "NetworkEngine.h"

void GameHandler::Update()
{
	if (NetworkEngine::Instance().IsServer())
	{
		//On RPC Client Connected
		
		//NoPlayers++;

		if (NoPlayers > 1 && !IsGameStarted())
			StartGame();
	}
}
