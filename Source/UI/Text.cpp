#include "Text.h"

Text::Text(std::string Id, std::wstring text, Vector2 pos, Vector2 scale, Vector4 color)
{
	this->Id = Id;
	this->TextValue = text;
	this->Position = pos;
	this->Scale = scale;
	Color = color;
}

void Text::Update(float deltaTime)
{
}