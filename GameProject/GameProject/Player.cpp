#include "GameCore.h"
#include "Player.h"
#include "Sprite.h"
#include "TextureAsset.h"
#include "BoxCollider.h"
#include "NetworkRPC.h"
#include "NetworkEngine.h"
#include "Bullet.h"
#define NDEBUG_PLAYER

IMPLEMENT_DYNAMIC_CLASS(Player)

void Player::Initialize()
{
	Component::Initialize();

	collider = owner->GetComponent<BoxCollider>();
	health = owner->GetComponent<Health>();
	RegisterRPC(GetHashCode("FireRPC"), std::bind(&Player::RPC, this, std::placeholders::_1));
}

void Player::Update()
{
	if (NetworkEngine::Instance().IsServer() == false)
	{
		const InputSystem& input = InputSystem::Instance();
		lastFireTime-=Time::Instance().DeltaTime();
		//shoot Test
		if (input.IsMouseButtonPressed(1)&&lastFireTime < 0.0f)
		{
			Fire();
			lastFireTime = cooldownTimer;
		}
	}

}
void Player::Load(json::JSON& node)
{
	Component::Load(node);


	if (node.hasKey("DeathScene"))
	{
		game_over_scene = GetHashCode(node.at("DeathScene").ToString().c_str());
	}
}

void Player::CheckCollision()
{
	for (const auto& other : collider->OnCollisionEnter())
	{
		if (other->GetOwner()->GetName() != "Enemy")
		{
			continue;
		}

		LOG("Player collided with enemy");
	}
}


void Player::Fire()
{
	if (NetworkEngine::Instance().IsClient())
	{
		Vec2 targetPos;
		int mouseX, mouseY;
		SDL_GetMouseState(&mouseX, &mouseY);
		targetPos = Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
		Vec2 direction = targetPos - owner->GetTransform().position;
		direction.Normalize();


		RakNet::BitStream bitStream;

		bitStream.Write((unsigned char)MSG_SCENE_MANAGER);
		bitStream.Write((unsigned char)MSG_RPC);

		bitStream.Write(owner->GetParentScene()->GetUid());
		bitStream.Write(owner->GetUid());

		bitStream.Write(GetUid());

		bitStream.Write(GetHashCode("FireRPC"));

		bitStream.Write(direction.x);
		bitStream.Write(direction.y);
		bitStream.Write(owner->GetTransform().position.x);
		bitStream.Write(owner->GetTransform().position.y);
		NetworkEngine::Instance().SendPacket(bitStream);
	}
}
void Player::RPC(RakNet::BitStream& bitStream)
{
	Vec2 direction;
	Vec2 position;
	float value = 0;
	bitStream.Read(value);
	direction.x = value;
	bitStream.Read(value);
	direction.y = value;
	bitStream.Read(value);
	position.x = value;
	bitStream.Read(value);
	position.y = value;

	Entity* bullet = SceneManager::Instance().CreateEntity();
	Sprite* bulletSprite = bullet->CreateComponent<Sprite>();
	TextureAsset* bulletTexture = (TextureAsset*)AssetManager::Instance().GetAsset("872a3acb-8431-4d8e-bed2-a330f447a98d");
	bulletSprite->SetTextureAsset(bulletTexture);

	std::vector<std::string> components = { "BoxCollider" };
	bullet->AddComponents(components);

	bullet->GetTransform().Scale(Vec2(1.3, 1.3));

	Bullet* bulletcomponent =bullet->CreateComponent<Bullet>();
	bullet->GetTransform().position = position;
	bulletcomponent->SetSpeed(100.0f);
	bulletcomponent->SetDirection(direction);

	float angleRadians = atan2(direction.y, direction.x);
	bullet->GetTransform().rotation = RAD_TO_DEG(angleRadians);


}
