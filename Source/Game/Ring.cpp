#include "Ring.h"
#include "Engine/IRenderable.h"

Ring::Ring(IRenderable* mainVisual, IRenderable* highlightVisual)
{
	MainVisual = mainVisual;
	HighlightVisual = highlightVisual;
	Visible = true;
}

void Ring::ToggleHighlight(bool highlight)
{
	HighlightVisual->SetVisible(highlight);
}

void Ring::SetPosition(Vector2 position)
{
	Position = position;
	MainVisual->SetPosition (position.x, position.y);
	HighlightVisual->SetPosition (position.x, position.y);
}

void Ring::SetScale(Vector2 scale)
{
	Scale = scale;
	MainVisual->SetScale(scale.x, scale.y);
	HighlightVisual->SetScale(scale.x, scale.y);
}

void Ring::SetRotation(float rotation)
{
	Rotation = rotation;
	MainVisual->SetRotation(Rotation);
}

void Ring::SetVisible(bool visible)
{
	MainVisual->SetVisible(visible);
	HighlightVisual->SetVisible(visible);
}