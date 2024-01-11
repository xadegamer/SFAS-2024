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

void AnimatedSprite::SetVisibilty(bool active)
{
	Visible = active;

	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetVisible(true);
	}
}

void AnimatedSprite::SetPosition(float x, float y)
{
	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetPosition(x, y);
	}
}

void AnimatedSprite::SetScale(float x, float y)
{
	for (int i = 0; i < FrameCount; i++)
	{
		Frames[i]->SetScale(x, y);
	}
}

float AnimatedSprite::GetXPosition()
{
	return Frames[CurrentFrame]->GetTransform().PositionX;
}

float AnimatedSprite::GetYPosition()
{
	return Frames[CurrentFrame]->GetTransform().PositionY;
}
