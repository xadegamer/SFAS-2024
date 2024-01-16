#pragma once

#include "IGameObject.h"
#include <functional>

class IGraphics;
class ITexture;
class IShader;
class IRenderable;
class AnimatedSprite;

class WaterTank : public IGameObject
{
private:

	IRenderable* TankBody;
	AnimatedSprite* TankWaterAnimation;
	IRenderable* TankMask;

	std::function <void()> OnFullEvent;
	std::function <void()> OnEmptyEvent;

	int FullWaterLevel;
	int NoWaterLevel;
	bool triggerdEvent;
	float waterSpeed;
	bool IsConnected;

	float waterSplashTimer;
	float waterSplashInterval;

public:

	WaterTank(IGraphics* Graphics);
	~WaterTank();

	virtual void SetPosition(Vector2 position) override;
	virtual void SetScale(Vector2 scale) override;
	virtual void SetRotation(float rotation) override;
	virtual void SetVisible(bool visible) override;

	void Update();

	void UpdateWaterLevel(float input);
	void ValidateWaterLevel();

	void SetWaterLevel(float level);

	void SplashWater();

	float GetNormalizedWaterLevel();

	void Reset();

	inline void SetWaterSpeed(float speed) { waterSpeed = speed; }
	inline float GetWaterSpeed() { return waterSpeed; }

	inline void SetOnFullEvent(std::function <void()> onFullEvent) { OnFullEvent = onFullEvent; }
	inline void SetOnEmptyEvent(std::function <void()> onEmptyEvent) { OnEmptyEvent = onEmptyEvent; }

	inline void SetIsConnected(bool isConnected) { IsConnected = isConnected; }
};