#pragma once

#include "Game/TypeHelpers.h"
#include "UIObject.h"
#include <string>
#include <iostream>

class IGraphics;
class ITexture;
class IShader;
class IRenderable;


class Image : public UIObject
{
private:

	IRenderable* sprite;

public:

	Image(std::string Id, IGraphics* Graphics, std::wstring texturePath, Vector2 pos = Vector2(0.0f, 0.0f), int layer = 11, Vector2 scale = Vector2(1.0f, 1.0f));
	~Image() = default;

	void Enable() override;
	void Disable() override;
	void SetPosition(Vector2 pos) override;
	void SetScale(Vector2 scale) override;

	void Update(float deltaTime) override;
};