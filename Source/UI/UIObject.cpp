#include "UIObject.h"

void UIObject::Enable()
{
	Enabled = true;
}

void UIObject::Disable()
{
	Enabled = false;
}

void UIObject::SetScale(Vector2 scale)
{
	Scale = scale;
}

void UIObject::SetPosition(Vector2 pos)
{
	Position = pos;
}

void UIObject::SetLayer(int layer)
{
	Layer = layer;
}
