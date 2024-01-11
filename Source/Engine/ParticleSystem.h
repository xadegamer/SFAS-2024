#pragma once

#include "Game/Particle.h"
#include "Engine/Pool.h"

#include <vector>

class IGraphics;

enum class ParticleDirection
{
	Random,
	Cicular,
	Up,
	Down,
	Right,
	Left,
};

class ParticleSystem
{
private:
	ParticleSystem() = delete;
	~ParticleSystem() = delete;

	static Pool<Particle>* ParticlePool;
	static IGraphics* Graphics;

public:

	static void Init(IGraphics* graphics);

	static void Update(float deltaTime);

	static void Emit(Vector2 position, Vector2 size, ParticleDirection direction, int amount, float speed, float lifeTime);

	static void Emit(Vector2 position, Vector2 size, Vector2 direction, int amount, float force, float lifeTime);

	static void Clear();

	static void Return(Particle* particle);

	static Particle* OnParticleCreate();

	static void OnParticleGetFromPool(Particle* particle);

	static void OnParticleReturnToPool(Particle* particle);

	static Vector2 GetDirection(ParticleDirection direction);

	static int RandomRangeInt(int min, int max);
};

