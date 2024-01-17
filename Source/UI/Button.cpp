#include "Button.h"
#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"

Button::Button(std::string id, IGraphics* graphics, Vector2 position, Vector2 scale)
{
	Id = id;
	Position = position;
	Scale = scale;
	Enabled = true;

	NormalImage = new Image("BG", graphics, L"Resource/Textures/Btn.dds", position, 13, Scale);
	HighlightImage = new Image("BG", graphics, L"Resource/Textures/Btn_H.dds", position, 12, Scale + Vector2(0.1f, 0.1f));
	//HighlightImage = new Image("BG", graphics, L"Resource/Textures/Btn_H.dds", position + Vector2(-10.0f, -10.0f), 12, Scale + Vector2(0.1f, 0.1f));

	HighlightImage->Disable();
}

void Button::Update(float deltaTime)
{
}

void Button::Enable()
{
	UIObject::Enable();
	NormalImage->Enable();
	HighlightImage->Enable();
	TextToDisplay->Enable();
}

void Button::Disable()
{
	UIObject::Disable();
	NormalImage->Disable();
	HighlightImage->Disable();
	TextToDisplay->Disable();
}

void Button::ToggleHighlight(bool state)
{
	if (state)
	{
		if (OnHighlight) OnHighlight();
		HighlightImage->Enable();
	}
	else
	{
		HighlightImage->Disable();
	}
}

void Button::Select()
{
	if (OnSelect) OnSelect();
}

void Button::AddText(std::string id, std::wstring text, Vector2 position, Vector2 size, Vector4 color)
{
	TextToDisplay = new Text(id, text, position, size);
}

void Button::SetText(std::wstring text)
{
	TextToDisplay->SetText(text);
}

void Button::AddSelectEventListener(std::function<void()> onSelect)
{
	OnSelect = onSelect;
}

void Button::AddHighlightEventListener(std::function<void()> onHighlight)
{
	OnHighlight = onHighlight;
}
