#include "GameCore.h"
#include "Asteroid.h"
#include "BoxCollider.h"
#include "NetworkEngine.h"

void Asteroid::Initialize()
{
	Component::Initialize();

	collider = (BoxCollider*)owner->GetComponent("BoxCollider");
	health = (Health*)owner->GetComponent("Health");
	RegisterRPC(GetHashCode("AstRPC"), std::bind(&Asteroid::RPC, this, std::placeholders::_1));

	direction = Vec2(0, 1);
}

void Asteroid::Update()
{
	Move();
	if (NetworkEngine::Instance().IsServer()) {
		updateTimer -= Time::Instance().DeltaTime();
		if (updateTimer <= 0.0f) {
			SendAsteroidUpdate(false);
			updateTimer = 1.0f; // Reset timer
		}
	}
	CheckCollision();
	CheckBounds();
}

void Asteroid::CheckBounds()
{
	if (owner->GetTransform().position.y > RenderSystem::Instance().GetWindowSize().y ||
		owner->GetTransform().position.y < 0 ||
		owner->GetTransform().position.x > RenderSystem::Instance().GetWindowSize().x ||
		owner->GetTransform().position.x < 0)
	{
		// Destroy or deactivate this bullet
		
		DestoryAsteroid();
		return;
	}

}

void Asteroid::SendAsteroidUpdate(bool shouldDestroy)
{
	if (NetworkEngine::Instance().IsServer())
	{
		RakNet::BitStream bitStream;


		bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
		bitStream.Write((unsigned char)MSG_RPC);

		bitStream.Write(owner->GetParentScene()->GetUid());
		bitStream.Write(owner->GetUid());

		bitStream.Write(GetUid());

		bitStream.Write(GetHashCode("AstRPC"));

		unsigned char actionFlag = shouldDestroy ? 1 : 0;
		bitStream.Write(actionFlag);

		// If not destroying, include position data
		if (!shouldDestroy) {
			bitStream.Write(owner->GetTransform().position.x);
			bitStream.Write(owner->GetTransform().position.y);
			bitStream.Write(Time::Instance().TotalTime());
		}

		// Send the packet
		NetworkEngine::Instance().SendPacket(bitStream);

		// If destroying, also remove the entity immediately on the server
		if (shouldDestroy) {
			SceneManager::Instance().RemoveEntity(owner->GetUid());
		}
	}
}

void Asteroid::RPC(RakNet::BitStream& bitStream)
{
	float value = 0;
	bitStream.Read(value);
	SceneManager::Instance().RemoveEntity(value);
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
			if (health == 0)
				DestoryAsteroid();
		}
		break;
	}
}


void Asteroid::DestoryAsteroid()
{
	RakNet::BitStream bitStream;

	bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	bitStream.Write((unsigned char)MSG_RPC);

	bitStream.Write(owner->GetParentScene()->GetUid());
	bitStream.Write(owner->GetUid());

	bitStream.Write(GetUid());

	bitStream.Write(GetHashCode("AstRPC"));
	bitStream.Write(owner->GetUid());
	NetworkEngine::Instance().SendPacket(bitStream);
	SceneManager::Instance().RemoveEntity(owner->GetUid());
	LOG("Asteroid Destroyed");
}

