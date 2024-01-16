#include "ParticleSystem.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"

Pool<Particle>* ParticleSystem::ParticlePool;
IGraphics* ParticleSystem::Graphics;

void ParticleSystem::Init(IGraphics* graphics)
{
	Graphics = graphics;

	ParticlePool = new Pool<Particle>(std::bind(&ParticleSystem::OnParticleCreate),
		std::bind(&ParticleSystem::OnParticleGetFromPool, std::placeholders::_1),
		std::bind(&ParticleSystem::OnParticleReturnToPool, std::placeholders::_1),
		200);

}

void ParticleSystem::Update(float deltaTime)
{
	for (size_t i = 0; i < ParticlePool->GetActivePoolObjects().size(); i++)
	{
		if (ParticlePool->GetActivePoolObjects()[i]->GetIsActive())
		{
			ParticlePool->GetActivePoolObjects()[i]->Update(deltaTime);
		}
	}
}

void ParticleSystem::Emit(Vector2 position, Vector2 size, ParticleDirection direction, int amount, float speed, float lifeTime)
{
	for (size_t i = 0; i < amount; i++)
	{
		Emit(position, size, GetDirection(direction), 1, speed, lifeTime);
	}
}

void ParticleSystem::Emit(Vector2 position, Vector2 size, Vector2 direction, int amount, float speed, float lifeTime)
{
	for (size_t i = 0; i < amount; i++)
	{
		Particle* newParticle = ParticlePool->Get();
		newParticle->Emit(position, size, direction, speed, lifeTime);
	}
}

void ParticleSystem::Clear()
{
	delete	ParticlePool;
	ParticlePool = nullptr;
}

void ParticleSystem::Return(Particle* particle)
{
	ParticlePool->Return(particle);
}

Particle* ParticleSystem::OnParticleCreate()
{
	ITexture* texture = Graphics->CreateTexture(L"Resource/Textures/100x Light Blue.dds");
	IShader* shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", texture);
	Particle* newParticle = new Particle(Graphics->CreateBillboard(shader, 10));;
	newParticle->SetActive(false);
	return newParticle;
}

void ParticleSystem::OnParticleGetFromPool(Particle* particle)
{
	particle->SetActive(true);
}

void ParticleSystem::OnParticleReturnToPool(Particle* particle)
{
	particle->SetActive(false);
}

Vector2 ParticleSystem::GetDirection(ParticleDirection direction)
{
	switch (direction)
	{
		case ParticleDirection::Random:
			return GetDirection(static_cast<ParticleDirection>(RandomRangeInt(1, 5)));
		case ParticleDirection::Cicular:
		{
			float angle = static_cast<float>(rand()) / static_cast<float>(RAND_MAX) * 2.0f * PI;
			return Vector2(cos(angle), sin(angle));;
		}
		case ParticleDirection::Up:
			return Vector2(0.0f, -1.0f);
		case ParticleDirection::Down:
			return Vector2(0.0f, 1.0f);
		case ParticleDirection::Right:
			return Vector2(1.0f, 0.0f);
		case ParticleDirection::Left:
			return Vector2(-1.0f, 0.0f);
		default:break;
	}
}

int ParticleSystem::RandomRangeInt(int min, int max)
{
	if (min > max)
		return RandomRangeInt(max, min);
	if (min == max)
		return min;
	return min + (rand() % (max - min));
}