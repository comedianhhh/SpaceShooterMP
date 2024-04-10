#include "GameCore.h"
#include "Bullet.h"
#include "SceneManager.h"
#include "NetworkEngine.h"
IMPLEMENT_DYNAMIC_CLASS(Bullet)

void Bullet::Initialize()
{
	Component::Initialize();
	collider = owner->GetComponent<BoxCollider>();
	RegisterRPC(GetHashCode("FireRPC"), std::bind(&Bullet::RPC, this, std::placeholders::_1));
}

void Bullet::Update()
{
	CheckBounds();
	Move();
	CheckCollision();
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
		// Destroy or deactivate this bullet
		DestoryBullet();
		
		return;
	}

}
void Bullet::CheckCollision()
{
	if (collider == nullptr) return;

	for (const auto& other : collider->OnCollisionEnter())
	{
		// Here, you can check for collision with specific entities
		if (other->GetOwner()->GetName() == "Enemy")
		{
			DestoryBullet();
			LOG("Bullet collided with enemy");
		}
		break;
	}
}
void Bullet::RPC(RakNet::BitStream& bitStream)
{
	float value = 0;
	bitStream.Read(value);
	SceneManager::Instance().RemoveEntity(value);
}
void Bullet::DestoryBullet()
{
	RakNet::BitStream bitStream;

	bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
	bitStream.Write((unsigned char)MSG_RPC);

	bitStream.Write(owner->GetParentScene()->GetUid());
	bitStream.Write(owner->GetUid());

	bitStream.Write(GetUid());

	bitStream.Write(GetHashCode("FireRPC"));
	bitStream.Write(owner->GetUid());
	NetworkEngine::Instance().SendPacket(bitStream);
	SceneManager::Instance().RemoveEntity(owner->GetUid());
	LOG("Bullet Destroyed");
}