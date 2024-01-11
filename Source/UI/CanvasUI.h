#pragma once

#include "UIObject.h"
#include "Game/TypeHelpers.h"
#include "vector"
#include "Button.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;

class CanvasUI
{
private:

	std::string Id;
	std::vector<UIObject*> UiObjects;
	bool Active;

	IGraphics* Graphics;

public:
	CanvasUI(IGraphics* Graphics, std::string id);
	~CanvasUI();

	void AddUIObject(UIObject* uiObject);

	void Update(float deltaTime);

	void SetEnabled(bool enabled);

	UIObject* GetUIObjectByID(std::string id);

	template <class T, typename = std::enable_if<std::is_base_of<UIObject, T>::value>>
	T* GetUIObjectByID(std::string id)
	{
		for (size_t i = 0; i < UiObjects.size(); i++)
		{
			if (UiObjects[i]->GetID() == id)
				return dynamic_cast<T*>(UiObjects[i]);
		}
		return nullptr;
	}

	template <class T, typename = std::enable_if<std::is_base_of<UIObject, T>::value>>
	std::vector<T*> GetUIObjectsOfType() 
	{
		std::vector<T*> uiObjects;
		for (size_t i = 0; i < UiObjects.size(); i++)
		{
			Button* button = dynamic_cast<Button*>(UiObjects[i]);
			if (button)
			{
				if (dynamic_cast<T*>(dynamic_cast<Button*>(UiObjects[i])->GetText()))
					uiObjects.push_back(dynamic_cast<T*>(button->GetText()));
			}

			if (dynamic_cast<T*>(UiObjects[i]))
				uiObjects.push_back(dynamic_cast<T*>(UiObjects[i]));
		}
		return uiObjects;
	}

	inline std::string GetID() { return Id; };
	inline bool IsEnabled() { return Active; };
};
