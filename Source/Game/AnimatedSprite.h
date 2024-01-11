#pragma once

#include <vector>

class ITexture;
class IShader;
class IRenderable;

class AnimatedSprite
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

	void SetVisibilty(bool active);
	void SetPosition(float x, float y);
	void SetScale(float x, float y);
	float GetXPosition();
	float GetYPosition();

	inline bool GetIsVisible() { return Visible; }
};

