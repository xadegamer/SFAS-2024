#pragma once

#include <iostream>
#include <string>
#include <functional>
#include <vector>

#include "CanvasUI.h"
#include "UIObject.h"
#include "Text.h"
#include "Image.h"	
#include "Button.h"
#include "ButtonNavigator.h"

class IGraphics;
class IInput;

class UISystem
{
private:
	UISystem() = delete;
	~UISystem() = delete;

	static CanvasUI* LastActiveCanvas;
	static CanvasUI* CurrentCanvas;

	static std::vector<CanvasUI*> Canvases;

	static IGraphics* Graphics;
	static IInput* Input;

public:

	static void Init(IGraphics* graphics, IInput* input);

	static void SetUpMainMenuCanvas();

	static void SetUpGameCanvas();

	static void PauseMenu();

	static void GameOverMenu();

	static void WinMenu();

	static void Update(float deltaTime);

	static void EnableCanvasByID(std::string id);

	static void EnableCanvas(CanvasUI* canvas);

	static void Clean();

	static CanvasUI* GetCanvasUIByID(std::string id);

	inline static CanvasUI* GetActiveCanvas() { return CurrentCanvas; }
	inline static CanvasUI* GetLastActiveCanvas() { return LastActiveCanvas; }
	inline static std::vector<CanvasUI*> GetCanvases() { return Canvases; }
};
