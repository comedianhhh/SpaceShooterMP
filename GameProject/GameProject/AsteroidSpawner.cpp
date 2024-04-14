#include "GameCore.h"
#include "AsteroidSpawner.h"
#include "Sprite.h"
#include "GameHandler.h"
#include "NetworkEngine.h"
#include "NetworkRPC.h"
#include "Asteroid.h"
#include <random>
#include "BoxCollider.h"

IMPLEMENT_DYNAMIC_CLASS(AsteroidSpawner)
void AsteroidSpawner::Initialize()
{
	Component::Initialize();
	srand((unsigned int)time(NULL));
}

void AsteroidSpawner::Update()
{
	if (NetworkEngine::Instance().IsServer()&&NetworkEngine::Instance().GetNumOfConnections()>1)
	{
		lastSpawnTime -= Time::Instance().DeltaTime();
		if (lastSpawnTime <= 0)
		{
			SpawnAsteroid();
			lastSpawnTime = frequency + rand() % 2;

		}
	}
}

void AsteroidSpawner::SpawnAsteroid()
{
	RakNet::BitStream bitStream;
	int type = rand() % 2;
	Vec2 position = GetRandomPosition();

	Entity* asteroid = SceneManager::Instance().CreateEntity();
	Sprite* asteroidSprite = asteroid->CreateComponent<Sprite>();
	BoxCollider* collidercomponent = asteroid->CreateComponent<BoxCollider>();
	asteroid->CreateComponent<Asteroid>();
	if (type == 0)
	{
		TextureAsset* asteroidTexture = (TextureAsset*)AssetManager::Instance().GetAsset("b4542c23-413d-4022-aeff-8b8f3cfb8277");
		asteroidSprite->SetTextureAsset(asteroidTexture);
	}
	else
	{
		TextureAsset* asteroidTexture = (TextureAsset*)AssetManager::Instance().GetAsset("0b0a0a2e-cf1f-4195-9ca5-9868558b0701");
		asteroidSprite->SetTextureAsset(asteroidTexture);
	}
	asteroid->GetTransform().position = position;

}
Vec2 AsteroidSpawner::GetRandomPosition() 
{
	float x = rand()% RenderSystem::Instance().GetWindowSize().x;
	float y = 10.0f;
	return Vec2(x, y);
}

