#include "GameCore.h"
#include "Bullet.h"
#include "SceneManager.h"
#include "NetworkEngine.h"
#include "Asteroid.h"
IMPLEMENT_DYNAMIC_CLASS(Bullet)

void Bullet::Initialize()
{
	Component::Initialize();
	collider = owner->GetComponent<BoxCollider>();
}

void Bullet::Update()
{
	Move();
	if (NetworkEngine::Instance().IsServer()) {
		CheckBounds();
		CheckCollision();
	}
}
void Bullet::Move()
{
	owner->GetTransform().position += direction * speed * Time::Instance().DeltaTime();
}

void Bullet::CheckBounds()
{
	if (owner->GetTransform().position.y > RenderSystem::Instance().GetWindowSize().y ||
		owner->GetTransform().position.y < 0 ||
		owner->GetTransform().position.x > RenderSystem::Instance().GetWindowSize().x ||
		owner->GetTransform().position.x < 0)
	{
		SceneManager::Instance().RemoveEntity(owner->GetUid());
		return;
	}

}
void Bullet::CheckCollision()
{
	if (collider == nullptr) return;
	for (const auto& other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->HasComponent<Asteroid>())
		{
			SceneManager::Instance().RemoveEntity(owner->GetUid());
		}
		break;
	}
}
void Bullet::SetDirection(const Vec2& dir)
{
	direction = dir;
}

void Bullet::SetSpeed(float spd)
{
	speed = spd;
}