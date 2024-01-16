#pragma once

#include "IGameObject.h"
#include <vector>

class ITexture;
class IShader;
class IRenderable;

class AnimatedSprite : public IGameObject
{
private:

	std::vector<IRenderable*> Frames;
	int FrameCount;
	int CurrentFrame;
	float Speed;
	float CurrentTime;

	bool CanPlay;
	bool IsPlaying;
	bool IsLooping;
	bool Visible;

public:

	AnimatedSprite(float Speed, bool play = false);
	~AnimatedSprite() = default;

	void AddFrame(IRenderable* Frame);
	void Update();
	void Play();
	void Stop();

	virtual void SetPosition(Vector2 position) override;
	virtual void SetScale(Vector2 scale) override;
	virtual void SetRotation(float rotation) override;
	virtual void SetVisible(bool visible) override;
};

