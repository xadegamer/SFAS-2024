#pragma once

#include <string>
#include "Game/TypeHelpers.h"

class UIObject
{

protected:
	std::string Id;
	int Layer = 0;
	bool Enabled;

	Vector2 Position;
	Vector2 Scale;

public:

	virtual void Update(float deltaTime) = 0;
	virtual void Enable();
	virtual void Disable();
	virtual void SetScale(Vector2 scale);
	virtual void SetPosition(Vector2 pos);
	virtual void SetLayer(int layer);

	inline bool IsEnabled() { return Enabled; };
	inline int GetLayer() { return Layer; };
	inline std::string GetID() { return Id; };
	inline Vector2 GetPosition() { return Position; }
	inline Vector2 GetScale() { return Scale; }
};