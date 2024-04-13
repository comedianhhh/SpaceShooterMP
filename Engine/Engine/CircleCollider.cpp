#include "EngineCore.h"
#include "Sprite.h"
#include "AnimatedSprite.h"
#include "CircleCollider.h"

IMPLEMENT_DYNAMIC_CLASS(CircleCollider);

void CircleCollider::Initialize()
{
	Component::Initialize();
	if (owner->HasComponent("Sprite"))
	{
		int height = ((Sprite*)(owner->GetComponent("Sprite")))->GetTargetRect().h;
		SetRadius(height);
	}
	else if (owner->HasComponent("AnimatedSprite"))
	{
		int height = ((AnimatedSprite*)(owner->GetComponent("AnimatedSprite")))->GetTargetRect().h;
		SetRadius(height);
	}
}