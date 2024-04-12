#include "GameCore.h"
#include "Asteroid.h"
#include "BoxCollider.h"
#include "NetworkEngine.h"
#include "Bullet.h"

IMPLEMENT_DYNAMIC_CLASS(Asteroid)
void Asteroid::Initialize()
{
	Component::Initialize();
	collider = (BoxCollider*)owner->GetComponent("BoxCollider");
	health = (Health*)owner->GetComponent("Health");
	direction = Vec2(0, 1);
}

void Asteroid::Update()
{
	if (NetworkEngine::Instance().IsServer()) 
	{
		CheckBounds();
		Move();
		CheckCollision();
	}
}

void Asteroid::CheckBounds()
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

void Asteroid::Move()
{
	owner->GetTransform().position += direction * speed * Time::Instance().DeltaTime();
}

void Asteroid::CheckCollision()
{
	if (collider == nullptr) return;
	LOG("Asteroid has collided!!");
	for (const auto& other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->HasComponent<Bullet>())
		{
			health -= 1;
			
			if (health == 0)
			{
				SceneManager::Instance().RemoveEntity(owner->GetUid());
			}
		}
		break;
	}
}




