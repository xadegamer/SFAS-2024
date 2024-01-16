#pragma once

#include "IGameObject.h"

class IRenderable;

class Ring : public IGameObject
{
	IRenderable* MainVisual;
	IRenderable* HighlightVisual;

public: 
		
		Ring(IRenderable* mainVisual, IRenderable* highlightVisual);
		virtual ~Ring() = default;

		void ToggleHighlight(bool highlight);

		virtual void SetPosition(Vector2 position) override;
		virtual void SetScale(Vector2 scale) override;
		virtual void SetRotation(float rotation) override;
		virtual void SetVisible(bool visible) override;
};