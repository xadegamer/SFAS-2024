#include "Text.h"

Text::Text(std::string Id, std::wstring text, Vector2 pos, Vector2 scale)
{
	SetEnabled(true);
	this->Id = Id;
	this->TextValue = text;
	this->Position = pos;
	this->Scale = scale;
	Color = DirectX::Colors::DarkRed;
}

void Text::Update(float deltaTime)
{
}
