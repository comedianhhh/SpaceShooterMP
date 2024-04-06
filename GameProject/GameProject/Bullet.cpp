#include "GameCore.h"
#include "Bullet.h"
#include "SceneManager.h"

IMPLEMENT_DYNAMIC_CLASS(Bullet)

void Bullet::Initialize()
{
	Component::Initialize();

	collider = (BoxCollider*)owner->GetComponent("BoxCollider");
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
		owner->GetParentScene()->RemoveEntity(owner->GetGuid());
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
			SceneManager::Instance().RemoveEntity(owner->GetUid());
		}
		break;
	}
}