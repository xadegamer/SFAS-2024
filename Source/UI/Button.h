#pragma once

#include "UIObject.h"
#include "Text.h"
#include  "Image.h"
#include <functional>

class IGraphics;

class Button : public UIObject
{

protected:

	Text* TextToDisplay;
	Image* NormalImage;
	Image* HighlightImage;

	std::function <void()> OnSelect;
	std::function <void()> OnHighlight;

public:

	Button(std::string id, IGraphics* graphics, Vector2 position, Vector2 scale);
	~Button() = default;

	void Update(float deltaTime) override;
	void Enable() override;
	void Disable() override;

	void ToggleHighlight(bool state);
	void Select();

	inline std::function <void()>& OnSelcted() { return OnSelect;}

	inline std::function <void()>& OnHighlighted() { return OnHighlight; }

	void AddText(std::string id, std::wstring text, Vector2 position, Vector2 size, Vector4 color);

	void SetText(std::wstring text);

	inline Text* GetText() { return TextToDisplay; }

	void AddSelectEventListener(std::function <void()> onSelect);
	void AddHighlightEventListener(std::function <void()> onHighlight);
};
