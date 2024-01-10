#pragma once
#include "Game/TypeHelpers.h"
#include "UIObject.h"
#include <string>
#include <iostream>


class Text : public UIObject
{
private:
	Vector4 Color;
	std::wstring TextValue;

public:

	Text(std::string Id, std::wstring text, Vector2 pos, Vector2 scale = Vector2(0.5f, 0.5f));
	~Text() = default;
	void Update(float deltaTime) override;

	inline std::wstring& GetText() { return TextValue; }
	inline void SetText(const std::wstring& text) { this->TextValue = text; }
	inline void SetText(const std::wstring&, Vector4 color) { this->TextValue = TextValue; this->Color = color; }

	inline Vector4 GetColor() { return Color; }
	inline void SetColor(Vector4 color) { this->Color = color; }
};
