#pragma once
#ifndef _HEALTH_H_
#define _HEALTH_H_
#include "GameCore.h"

class Health : public Component
{
	DECLARE_DYNAMIC_DERIVED_CLASS(Health, Component)

public:
	void Initialize() override;
	void Update() override;
	void Load(json::JSON&) override;
private:
	float currenthealth = 5;
	float maxhealth = 5;
	bool isDead = false;
	Vec2 originalScale = Vec2(1.0f, 1.0f);
	SDL_Color hit_effect_color = { 255,0,0,100 };
	SDL_Color health_color = { 255, 255, 255 ,255 };

public:
	bool CheckDead() { return isDead; }
	void SetHealth(float hp);
	float GetHealth() const;
	void SetMaxHealth(float hp);
	float GetMaxHealth() const;
	void TakeDamage(float dmg);
	void Heal(float hp);
	void ResetHealth();
	void TriggerHitEffect();
	void OnDeath();
	void AddHealth(float hp) { currenthealth += hp; }
};


#endif // !_HEALTH_H_