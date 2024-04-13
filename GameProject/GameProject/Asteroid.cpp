#include "GameCore.h"
#include "Asteroid.h"
#include "BoxCollider.h"
#include "NetworkEngine.h"
#include "Bullet.h"
#include "Player.h"

IMPLEMENT_DYNAMIC_CLASS(Asteroid)
void Asteroid::Initialize()
{
	Component::Initialize();
	collider = owner->GetComponent<BoxCollider>();
	direction = Vec2(0, 1);
	RegisterRPC(GetHashCode("AstRPC"), std::bind(&Asteroid::RPC, this, std::placeholders::_1));
}

void Asteroid::Update()
{
	if (NetworkEngine::Instance().IsServer()) 
	{
		CheckBounds();
		Move();
		updateTimer -= Time::Instance().DeltaTime();
		if (updateTimer <= 0.0f) {
			SendUpdate();
			updateTimer = 1.0f; // Reset timer
		}
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
	for (const auto& other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->HasComponent<Bullet>())
		{
			health-=1;
			if (health<=0)
			{
				SceneManager::Instance().RemoveEntity(owner->GetUid());
			}
		}
		else if (other->GetOwner()->HasComponent<Player>())
		{
			SceneManager::Instance().RemoveEntity(other->GetOwner()->GetUid());
			RakNet::BitStream bs;
			bs.Write((unsigned char)MSG_SCENE_MANAGER);
			bs.Write((unsigned char)MSG_GAME_OVER);
			bs.Write("GameOVer");
			NetworkEngine::Instance().SendPacket(bs);
		}
		break;
	}
}

void Asteroid::RPC(RakNet::BitStream& bitStream)
{
	unsigned char actionFlag;
	bitStream.Read(actionFlag);

	if (actionFlag == 1) { // 1 indicates destroy
		SceneManager::Instance().RemoveEntity(owner->GetUid());
	}
	else { // 0 indicates an update
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
}

void Asteroid::SendUpdate()
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



