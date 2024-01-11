#include "Particle.h"
#include "Engine/IRenderable.h"
#include "Engine/Time.h"
#include "Engine/ParticleSystem.h"

Particle::Particle(IRenderable* sprite) : Sprite(sprite), Direction(), LifeTime(), CurrentlifeTime(), IsActive(), Speed()
{

}

void Particle::Emit(Vector2 position, Vector2 scale, Vector2 direction, float speed, float lifeTime)
{
	Sprite->SetPosition(position.x, position.y);
	Sprite->SetScale(scale.x, scale.y);
	Direction = direction;
	LifeTime = CurrentlifeTime = lifeTime;
	Speed = speed;
	IsActive = true;
}

void Particle::Update(float deltaTime)
{
	if (CurrentlifeTime > 0)
	{
		CurrentlifeTime -= deltaTime;

		float scaledSpeed = Speed * deltaTime;

		float normalisedTime = CurrentlifeTime / LifeTime;
		Vector2 nextPosition = Vector2(Sprite->GetTransform().PositionX + (Direction.x * scaledSpeed), Sprite->GetTransform().PositionY + (Direction.y * scaledSpeed));
		Sprite->SetPosition(nextPosition.x, nextPosition.y);

		float nextScale = Sprite->GetTransform().ScaleX * normalisedTime;
		Sprite->SetScale(nextScale, nextScale);
	}
	else
	{
		IsActive = false;
		ParticleSystem::Return(this);
	}
}

void Particle::SetActive(bool status)
{
	IsActive = status;
	Sprite -> SetVisible(status);
}
