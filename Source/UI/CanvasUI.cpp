#include "CanvasUI.h"
#include "UIObject.h"

#include "Engine/IGraphics.h"
#include "Engine/IRenderable.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"

CanvasUI::CanvasUI(IGraphics* Graphics, std::string id, bool isActive)
{
	Id = id;
	Active = isActive;
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
	for (size_t i = 0; i < UiObjects.size(); i++)
	{
		UiObjects[i]->Update(deltaTime);
	}
}
