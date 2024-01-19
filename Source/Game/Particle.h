#pragma once
#include "TypeHelpers.h"

class IRenderable;

class Particle
{
private:

	IRenderable* Sprite;

	Vector2 Scale;
	Vector2 Direction;
	float LifeTime;
	float CurrentlifeTime;
	bool IsActive;
	float Speed;

public:

	Particle(IRenderable* sprite);
	~Particle() = default;

	void Emit(Vector2 position, Vector2 scale, Vector2 direction, float speed, float lifeTime);
	void Update(float deltaTime);
	void SetActive(bool status);
	inline bool GetIsActive() { return IsActive; }
};