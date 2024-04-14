#include "GameCore.h"
#include "GameHandler.h"
#include "NetworkEngine.h"
#include "Sprite.h"
#include "BoxCollider.h"

IMPLEMENT_DYNAMIC_CLASS(GameHandler)
void GameHandler::Initialize()
{
	Component::Initialize();
	RegisterRPC(GetHashCode("RPC"), std::bind(&GameHandler::RPC, this, std::placeholders::_1));
	srand((unsigned int)time(NULL));
}

void GameHandler::Update() 
{
	if (NetworkEngine::Instance().IsClient() && !bPlayerSpawned&&NetworkEngine::Instance().GetNumOfConnections()==1) 
	{
		if (spawnDelay > 0) {
			spawnDelay -= Time::Instance().DeltaTime();
		}
		else {
			SpawnPlayer();
			bPlayerSpawned = true;  
		}
	}
}

void GameHandler::SpawnPlayer()
{
	Vec2 newPosition = GenerateUniformPosition();
	bPlayerSpawned = true;
	RakNet::BitStream bitStream;

	bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	bitStream.Write((unsigned char)MSG_RPC);
	bitStream.Write(owner->GetParentScene()->GetUid());
	bitStream.Write(owner->GetUid());
	bitStream.Write(GetUid());
	bitStream.Write(GetHashCode("RPC"));
	bitStream.Write(newPosition.x);
	bitStream.Write(newPosition.y);
	bitStream.Write(NetworkEngine::Instance().rakInterface->GetMyGUID());
	NetworkEngine::Instance().SendPacket(bitStream);
}


void GameHandler::RPC(RakNet::BitStream& bitStream) 
{
	Vec2 position;
	float value = 0;
	uint64_t id;
	bitStream.Read(value);
	position.x = value;
	bitStream.Read(value);
	position.y = value;
	bitStream.Read(id);

	Entity* newplayer = SceneManager::Instance().CreateEntity();
	Sprite* playersprite = newplayer->CreateComponent<Sprite>();
	TextureAsset* playerTexture = (TextureAsset*)AssetManager::Instance().GetAsset("a7af9547-0ca5-4df0-8f37-276d7827117b");
	playersprite->SetTextureAsset(playerTexture);
	newplayer->CreateComponent<BoxCollider>();
	newplayer->GetTransform().Scale(Vec2(0.5, 0.5));
	Player* playercomponent = newplayer->CreateComponent<Player>();
	playercomponent->SetID(id);
	newplayer->GetTransform().position = position;
}

Vec2 GameHandler::GenerateUniformPosition() {
	int screenWidth = RenderSystem::Instance().GetWindowSize().x;
	int screenHeight = RenderSystem::Instance().GetWindowSize().y;
	int gridCols = 4;
	int cellWidth = screenWidth / gridCols;
	int cellX = rand() % gridCols;
	// Random position within the selected cell
	Vec2 newPosition = Vec2(
		cellX * cellWidth + rand() % cellWidth,
		screenHeight*0.8
	);

	return newPosition;
}

