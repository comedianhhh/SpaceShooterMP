#include "GameCore.h"
#include "Asteroid.h"
#include "BoxCollider.h"

void Asteroid::Initialize()
{
	Component::Initialize();

	collider = (BoxCollider*)owner->GetComponent("BoxCollider");
	health = (Health*)owner->GetComponent("Health");
	RegisterRPC(GetHashCode("RPC"), std::bind(&Asteroid::RPC, this, std::placeholders::_1));

	direction = Vec2(0, 1);
}

void Asteroid::Update()
{
	Move();
	CheckCollision();
}

void Asteroid::CheckBounds()
{
	if (owner->GetTransform().position.y > RenderSystem::Instance().GetWindowSize().y ||
		owner->GetTransform().position.y < 0 ||
		owner->GetTransform().position.x > RenderSystem::Instance().GetWindowSize().x ||
		owner->GetTransform().position.x < 0)
	{
		// Destroy or deactivate this bullet
		owner->GetParentScene()->RemoveEntity(owner->GetGuid());
		return;
	}

}


void Asteroid::RPC(RakNet::BitStream& bitStream)
{
}

void Asteroid::Move()
{
	owner->GetTransform().position +=  direction * speed * Time::Instance().DeltaTime();
}

void Asteroid::CheckCollision()
{
	if (collider == nullptr) return;

	for (const auto& other : collider->OnCollisionEnter())
	{
		// Here, you can check for collision with specific entities
		if (other->GetOwner()->GetName() == "Bullet")
		{
			health -= 1;
			if(health == 0)
				SceneManager::Instance().RemoveEntity(owner->GetUid());
		}
		break;
	}
}
