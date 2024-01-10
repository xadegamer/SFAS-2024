#include "CanvasUI.h"

CanvasUI::CanvasUI(std::string id, bool isActive)
{
	Id = id;
	Active = isActive;
	UiObjects = std::vector<UIObject*>();
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
