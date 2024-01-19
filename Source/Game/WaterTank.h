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
	IRenderable* TankWaterBg;
	IRenderable* TankWaterOverlay;
	IRenderable* TankBodyMask;
	IRenderable* TankLowerMask;

	IRenderable* WaterLevelMarker1;
	IRenderable* WaterLevelMarker2;
	IRenderable* WaterLevelMarker3;

	IRenderable* ClockBG;
	IRenderable* ClockNeedle;


	std::function <void()> OnFullEvent;
	std::function <void()> OnEmptyEvent;

	int FullWaterLevel;
	int NoWaterLevel;
	bool triggerdEvent;
	bool IsConnected;

	float waterSplashTimer;
	float waterSplashInterval;

	float ClockOffset;
	float ClockNeedleRotationOffset;
	float WaterOffset;

public:

	WaterTank(IGraphics* Graphics, bool start);
	~WaterTank();

	virtual void SetPosition(Vector2 position) override;
	virtual void SetScale(Vector2 scale) override;
	virtual void SetRotation(float rotation) override;
	virtual void SetVisible(bool visible) override;

	void Update();

	void UpdateWaterLevel(float input, float speed);
	void ValidateWaterLevel();

	void SetWaterLevel(float level);

	void SplashWater();

	void SetClockRotation();

	float GetNormalizedWaterLevel();

	float GetWaterPosition(float normalizedWaterLevel);

	void FillTank();

	void EmptyTank();

	void Reset();

	inline void SetOnFullEvent(std::function <void()> onFullEvent) { OnFullEvent = onFullEvent; }
	inline void SetOnEmptyEvent(std::function <void()> onEmptyEvent) { OnEmptyEvent = onEmptyEvent; }

	inline void SetIsConnected(bool isConnected) { IsConnected = isConnected; }
};