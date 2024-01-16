#pragma once
#include "TypeHelpers.h"

class IGameObject
{
protected:

	bool Visible;
	Vector2 Position;
	Vector2 Scale;
	float Rotation;

public:

	virtual void SetPosition(Vector2 position) = 0;
	virtual void SetScale(Vector2 scale) = 0;
	virtual void SetRotation(float rotation) = 0;
	virtual void SetVisible(bool visible) = 0;

	inline float GetXPosition() { return Position.x; }
	inline float GetYPosition() { return Position.y; }
	inline float GetRotation() { return Rotation; }
	inline bool GetIsVisible() { return Visible; }
};