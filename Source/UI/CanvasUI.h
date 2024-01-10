#pragma once

#include "UIObject.h"
#include "Game/TypeHelpers.h"
#include "vector"

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
	CanvasUI(IGraphics* Graphics, std::string id, bool isActive = false);
	~CanvasUI();

	void AddUIObject(UIObject* uiObject);

	void Update(float deltaTime);

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
			if (dynamic_cast<T*>(UiObjects[i]))
				uiObjects.push_back(dynamic_cast<T*>(UiObjects[i]));
		}
		return uiObjects;
	}

	inline std::string GetID() { return Id; };
	inline bool IsEnabled() { return Active; };
	inline void SetEnabled(bool enabled) { Active = enabled; };
};
