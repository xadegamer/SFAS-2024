#pragma once

#include "Button.h"
#include "Engine/IInput.h"
#include <vector>
#include "UIObject.h"


class ButtonNavigator : public UIObject
{
private:

	std::vector<Button*> Buttons;
	int CurrentButtonIndex;
	IInput* Input;

public:

	ButtonNavigator(IInput* input);
	ButtonNavigator() = default;

	void Update(float deltaTime) override;

	void AddButton(Button* button);
	void Navigate(int direction);
};
