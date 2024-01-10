
#pragma once

#include "Engine/IGraphics.h"
#include "Engine/Transform2D.h"

class IRenderable
{
public:

	virtual void Update() = 0;

	const Transform2D& GetTransform() const;
	void SetPosition(float x, float y);
	void SetRotation(float r);
	void SetScale(float sx, float sy);
	void SetVisible(bool visible);

	inline int GetLayer() const { return Layer; }
	inline void SetLayer(int layer) { this->Layer = layer; }
protected:
	
	Transform2D Transform;
	bool IsVisible = true;
	int Layer = 1;
};

