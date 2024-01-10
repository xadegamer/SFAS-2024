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

	inline bool IsEnabled() { return Enabled; };
	inline void SetEnabled(bool enabled) { Enabled = enabled; };

	inline void SetLayer(int layer) { Layer = layer; };
	inline int GetLayer() { return Layer; };

	inline std::string GetID() { return Id; };

	inline Vector2& GetPosition() { return Position; }
	inline void SetPosition(const Vector2& pos) { Position = pos; }

	inline Vector2& GetScale() { return Scale; }
	inline void SetScale(const Vector2& scale) { this->Scale = scale; }
};