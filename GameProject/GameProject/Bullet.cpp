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
	RegisterRPC(GetHashCode("FireRPC"), std::bind(&Bullet::RPC, this, std::placeholders::_1));
}

void Bullet::Update()
{
	if (NetworkEngine::Instance().IsServer()) {
		CheckBounds();
		Move();
		updateTimer -= Time::Instance().DeltaTime();
		if (updateTimer <= 0.0f) {
			SendBulletUpdate();
			updateTimer = 1.0f; // Reset timer
		}
		CheckCollision();
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
		// Here, you can check for collision with specific entities
		if (other->GetOwner()->HasComponent<Asteroid>())
		{
			SceneManager::Instance().RemoveEntity(owner->GetUid());
		}
		break;
	}
}
void Bullet::RPC(RakNet::BitStream& bitStream)
{
	float x, y;
	bitStream.Read(x);
	bitStream.Read(y);
	float servertime;
	bitStream.Read(servertime);
	float currentTime = Time::Instance().TotalTime();
	float timerdiff = currentTime - servertime;
	Vec2 newPosition = owner->GetTransform().position + direction * speed * timerdiff;
	owner->GetTransform().position = newPosition;
}


void Bullet::SendBulletUpdate()
{
	if (NetworkEngine::Instance().IsServer())
	{
		RakNet::BitStream bitStream;


		bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
		bitStream.Write((unsigned char)MSG_RPC);

		bitStream.Write(owner->GetParentScene()->GetUid());
		bitStream.Write(owner->GetUid());

		bitStream.Write(GetUid());

		bitStream.Write(GetHashCode("FireRPC"));

		bitStream.Write(owner->GetTransform().position.x);
		bitStream.Write(owner->GetTransform().position.y);
		bitStream.Write(Time::Instance().TotalTime());

		NetworkEngine::Instance().SendPacket(bitStream);
	}
}
