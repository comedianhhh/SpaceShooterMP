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

	collider = (BoxCollider*)owner->GetComponent("BoxCollider");
	health = (Health*)owner->GetComponent("Health");
	RegisterRPC(GetHashCode("RPC"), std::bind(&Player::RPC, this, std::placeholders::_1));
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

void Player::RPC(RakNet::BitStream& bitStream)
{
	float value = 0.0f;
	bitStream.Read(value);
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
	LOG("Player fired");
	Entity* bullet = owner->GetParentScene()->CreateEntity();
	Sprite* bulletSprite = (Sprite*)bullet->CreateComponent("Sprite");
	TextureAsset* bulletTexture = (TextureAsset*)AssetManager::Instance().GetAsset("872a3acb-8431-4d8e-bed2-a330f447a98d");
	bulletSprite->SetTextureAsset(bulletTexture);

	std::vector<std::string> components = { "BoxCollider" };
	bullet->AddComponents(components);
	bullet->GetTransform().position = owner->GetTransform().position;
	bullet->GetTransform().Scale(Vec2(1.3, 1.3));


	Vec2 targetPos;
	// Get mouse position and adjust by camera position.
	int mouseX, mouseY;
	SDL_GetMouseState(&mouseX, &mouseY);
	targetPos = Vec2(static_cast<float>(mouseX), static_cast<float>(mouseY));
	// Calculate direction and set velocity.
	Vec2 direction = targetPos - owner->GetTransform().position;
	direction.Normalize();
	float angleRadians = atan2(direction.y, direction.x);


	bullet->GetTransform().rotation = RAD_TO_DEG(angleRadians);

	// Assuming bullet has a ProjectileComponent to set its velocity.
	Bullet* bulletcomponent = (Bullet*)bullet->CreateComponent("Bullet");
	bulletcomponent->SetSpeed(100.0f);
	bulletcomponent->SetDirection(direction);

}

