#include "GameCore.h"
#include "AsteroidSpawner.h"
#include "Sprite.h"
#include "GameHandler.h"
#include <random>

void AsteroidSpawner::Initialize()
{
}

void AsteroidSpawner::Update()
{
	if (!GameHandler::Instance().IsGameStarted())
		return;

	lastSpawnTime -= Time::Instance().DeltaTime();

	if (lastSpawnTime <= 0)
		SpawnAsteroid();
}

void AsteroidSpawner::SpawnAsteroid()
{
	lastSpawnTime = frequency + GetRandom(-1.f, 2.f);

	LOG("Asteroid Spawned");
	Entity* asteroid = owner->GetParentScene()->CreateEntity();
	Sprite* asteroidSprite = (Sprite*)asteroid->CreateComponent("Enemy");
	TextureAsset* asteroidTexture = (TextureAsset*)AssetManager::Instance().GetAsset("872a3acb-8431-4d8e-bed2-a330f447a98d");	//@TODO Change this to sprite asset
	asteroidSprite->SetTextureAsset(asteroidTexture);

	asteroid->GetTransform().position = Vec2(GetRandom(0.f, RenderSystem::Instance().GetWindowSize().x), 0);
}

float AsteroidSpawner::GetRandom(float min, float max)
{
	/*std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> dis(min, max);

	float random_number = dis(gen);*/
	return 1.f;
}
