#pragma once

#include "Engine/IApplication.h"
#include <functional>

class IGraphics;
class ITexture;
class IShader;
class IRenderable;

class WaterTank
{
private:

	IRenderable* TankBody;
	IRenderable* TankFill;
	IRenderable* TankMask;

	std::function <void()> OnFullEvent;
	std::function <void()> OnEmptyEvent;

	int FullWaterLevel;
	int NoWaterLevel;
	bool triggerdEvent;

	float waterSpeed;

public:

	WaterTank(IGraphics* Graphics);
	virtual ~WaterTank();

	void SetPosition(float x, float y);
	void SetScale(float x, float y);;

	void UpdateWaterLevel(float input);
	void ValidateWaterLevel();

	void SetWaterLevel(float level);

	inline void SetWaterSpeed(float speed) { waterSpeed = speed; }
	inline float GetWaterSpeed() { return waterSpeed; }

	inline void SetOnFullEvent(std::function <void()> onFullEvent) { OnFullEvent = onFullEvent; }
	inline void SetOnEmptyEvent(std::function <void()> onEmptyEvent) { OnEmptyEvent = onEmptyEvent; }
};