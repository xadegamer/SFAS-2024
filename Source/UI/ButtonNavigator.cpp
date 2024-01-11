#include "ButtonNavigator.h"

#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

ButtonNavigator::ButtonNavigator(IInput* input)
{
	Input = input;
	CurrentButtonIndex = 0;
}

void ButtonNavigator::Update(float deltaTime)
{
	if (Input->IsPressed(InputAction::DirectionPadTop))
	{
		Navigate (-1);
	}
	else if (Input->IsPressed(InputAction::DirectionPadBottom))
	{
		Navigate(1);
	}
	else if (Input->IsPressed(InputAction::ButtonBottom))
	{
		Buttons[CurrentButtonIndex]->Select();
	}
}

void ButtonNavigator::Enable()
{
	UIObject::Enable();

	for (size_t i = 0; i < Buttons.size(); i++)
	{
		Buttons[i]->ToggleHighlight(i == CurrentButtonIndex);
	}
}

void ButtonNavigator::AddButton(Button* button)
{
	Buttons.push_back(button);
	button->ToggleHighlight(false);
}

void ButtonNavigator::Navigate(int direction)
{
	int previousButtonIndex = CurrentButtonIndex;
	int nextButtonIndex = CLAMP(previousButtonIndex + direction, 0, Buttons.size() - 1);
	if (previousButtonIndex == nextButtonIndex) return;

	Buttons[CurrentButtonIndex]->ToggleHighlight(false);
	CurrentButtonIndex = nextButtonIndex;
	Buttons[CurrentButtonIndex]->ToggleHighlight(true);
}
