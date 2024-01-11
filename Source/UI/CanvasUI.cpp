#include "CanvasUI.h"
#include "UIObject.h"

#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"

CanvasUI::CanvasUI(IGraphics* Graphics, std::string id)
{
	Id = id;
	Active = false;
	UiObjects = std::vector<UIObject*>();
	this->Graphics = Graphics;
}

CanvasUI::~CanvasUI()
{
	for (size_t i = 0; i < UiObjects.size(); i++)
	{
		delete UiObjects[i];
	}
	UiObjects.clear();
}

void CanvasUI::AddUIObject(UIObject* uiObject)
{
	UiObjects.push_back(uiObject);
}

void CanvasUI::Update(float deltaTime)
{
	if (!Active) return;
	for (size_t i = 0; i < UiObjects.size(); i++)
	{
		UiObjects[i]->Update(deltaTime);
	}
}

void CanvasUI::SetEnabled(bool enabled)
{
	Active = enabled;
	for (size_t i = 0; i < UiObjects.size(); i++)
	{
		if (enabled)
			UiObjects[i]->Enable();
		else
			UiObjects[i]->Disable();
	}
}
