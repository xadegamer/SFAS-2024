#include "AnimatedSprite.h"

#include "Engine/IRenderable.h"
#include "Engine/Time.h"

AnimatedSprite::AnimatedSprite(float Speed, bool play)
{
	FrameCount = 0;
	CurrentFrame = 0;
	CurrentTime = 0;
	this->Speed = Speed;
	CanPlay = play;
	Visible = true;
}

void AnimatedSprite::AddFrame(IRenderable* Frame)
{
	Frames.push_back(Frame);
	FrameCount++;

	if (FrameCount == 1)
	{
		Frame->SetVisible(true);
	}
	else
	{
		Frame->SetVisible(false);
	}
}

void AnimatedSprite::Update()
{
	if (!Visible || !CanPlay)
	{
		if (CurrentFrame != 0)
		{
			Frames[CurrentFrame]->SetVisible(false);
			CurrentFrame = 0;
			Frames[CurrentFrame]->SetVisible(true);
		}

		return;
	}

	CurrentTime += Time:: GetDeltaTime();
	if (CurrentTime > Speed)
	{
		CurrentTime = 0;
		Frames [CurrentFrame]->SetVisible(false);
		CurrentFrame = (CurrentFrame + 1) % FrameCount;
		Frames [CurrentFrame]->SetVisible(true);
	}
}

void AnimatedSprite::Play()
{
	CanPlay = true;
}

void AnimatedSprite::Stop()
{
	CanPlay = false;
}

void AnimatedSprite::SetPosition(Vector2 position)
{
	Position = position;
	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetPosition(position.x, position.y);
	}
}

void AnimatedSprite::SetScale(Vector2 scale)
{
	Scale = scale;
	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetScale(scale.x, scale.y);
	}
}

void AnimatedSprite::SetRotation(float rotation)
{
	Rotation = rotation;
	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetRotation(rotation);
	}
}

void AnimatedSprite::SetVisible(bool visible)
{
	Visible = visible;

	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetVisible(true);
	}
}