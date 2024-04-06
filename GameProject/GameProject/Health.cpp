#include "GameCore.h"
#include "Health.h"
#include "Time.h"
#include "AnimatedSprite.h"

IMPLEMENT_DYNAMIC_CLASS(Health)

void Health::Initialize()
{
	Component::Initialize();
	currenthealth = maxhealth;
	originalScale = owner->GetTransform().scale;
}

void Health::Update()
{
	if (currenthealth <= 0) 
	{
		isDead = true;
	}
}

void Health::Load(json::JSON& node)
{
	Component::Load(node);
	if (node.hasKey("MaxHealth"))
	{
		maxhealth = node["MaxHealth"].ToFloat();
	}
}

void Health::SetHealth(float hp)
{
	currenthealth = hp;
}

void Health::SetMaxHealth(float Maxhp)
{
	currenthealth = Maxhp;
}

float Health::GetHealth() const
{
	return currenthealth;
}

float Health::GetMaxHealth() const
{
	return maxhealth;
}

void Health::OnDeath()
{

}

void Health::TakeDamage(float dmg)
{
	currenthealth -= dmg;
	TriggerHitEffect();
	LOG(owner->GetGuid() << "  Hurt!")
}

void Health::Heal(float hp)
{
	currenthealth += hp;
}
void Health::ResetHealth()
{
	currenthealth = maxhealth;
}
void Health::TriggerHitEffect()
{


}