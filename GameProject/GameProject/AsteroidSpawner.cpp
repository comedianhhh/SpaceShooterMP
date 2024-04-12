#include "GameCore.h"
#include "AsteroidSpawner.h"
#include "Sprite.h"
#include "GameHandler.h"
#include "NetworkEngine.h"
#include "NetworkRPC.h"
#include "Asteroid.h"
#include <random>

void AsteroidSpawner::Initialize()
{
	RegisterRPC(GetHashCode("AstRPC"), std::bind(&AsteroidSpawner::RPC, this, std::placeholders::_1));

}

void AsteroidSpawner::Update()
{
	if (NetworkEngine::Instance().IsServer())
	{
		if (!GameHandler::Instance().IsGameStarted())
			return;

		lastSpawnTime -= Time::Instance().DeltaTime();

		if (lastSpawnTime <= 0)
			SpawnAsteroid();
	}
}

void AsteroidSpawner::SpawnAsteroid()
{
	RakNet::BitStream bitStream;

	bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	bitStream.Write((unsigned char)MSG_RPC);

	bitStream.Write(owner->GetParentScene()->GetUid());
	bitStream.Write(owner->GetUid());

	bitStream.Write(GetUid());

	bitStream.Write(GetHashCode("AstRPC"));

	bitStream.Write(GetRandom(0.f, RenderSystem::Instance().GetWindowSize().x));
	bitStream.Write(0);
	NetworkEngine::Instance().SendPacket(bitStream);

	lastSpawnTime = frequency + GetRandom(-1.f, 2.f);

}

float AsteroidSpawner::GetRandom(float min, float max)
{
	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);

	float random_number = dis(gen);*/
	return 1.f;
}

void AsteroidSpawner::RPC(RakNet::BitStream& bitStream)
{
	Vec2 position;

	float value;

	bitStream.Read(value);
	position.x = value;
	bitStream.Read(value);
	position.y = value;

	LOG("Asteroid Spawned");
	Entity* asteroid = SceneManager::Instance().CreateEntity();
	Sprite* asteroidSprite = (Sprite*)asteroid->CreateComponent("Enemy");
	TextureAsset* asteroidTexture = (TextureAsset*)AssetManager::Instance().GetAsset("872a3acb-8431-4d8e-bed2-a330f447a98d");	//@TODO Change this to sprite asset
	asteroidSprite->SetTextureAsset(asteroidTexture);

	asteroid->GetTransform().position;
}
