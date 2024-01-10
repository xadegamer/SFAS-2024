#include "UISystem.h"
#include "CanvasUI.h"
#include "UIObject.h"
#include "Text.h"
#include "Image.h"	

#include "Engine/IGraphics.h"

std::vector<CanvasUI*> UISystem::Canvases;
CanvasUI* UISystem::LastActiveCanvas = nullptr;
CanvasUI* UISystem::CurrentCanvas = nullptr;
IGraphics* UISystem::Graphics = nullptr;

const uint32_t kDisplayBufferWidth = 1920 / 2;
const uint32_t kDisplayBufferHeight = 1080 / 2;

const Vector2 Screen_Mid = Vector2(kDisplayBufferWidth / 2, kDisplayBufferHeight / 2);
const Vector2 Screen_TopRightCorner = Vector2(kDisplayBufferWidth, 0);
const Vector2 Screen_TopLeftCorner = Vector2(0, 0);
const Vector2 Screen_BottomRightCorner = Vector2(kDisplayBufferWidth, kDisplayBufferHeight);
const Vector2 Screen_BottomLeftCorner = Vector2(0, kDisplayBufferHeight);

void UISystem::Init(IGraphics* graphics)
{
	Graphics = graphics;
	SetUpGameCanvas();
}

void UISystem::SetUpMainMenuCanvas()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* mainMenuCanvas = new CanvasUI(Graphics,"MainMenuCanvas", true);
	Canvases.push_back(mainMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/BG.dds");
	mainMenuCanvas->AddUIObject(bg);

	Text* text = new Text("TitleText", L"Space Invaders", Vector2(0.0f, 0.0f), Vector2(0.5f, 0.5f));
	mainMenuCanvas->AddUIObject(text);

	CurrentCanvas = mainMenuCanvas;
}

void UISystem::SetUpGameCanvas()
{
	CanvasUI* gameCanvas = new CanvasUI(Graphics,"GameCanvas", true);
	Canvases.push_back(gameCanvas);
	Image* bg = new Image("BG", Graphics, L"Resource/Textures/BG.dds");
	gameCanvas->AddUIObject(bg);
	Text* text = new Text("ScoreText", L"THE GAME", Screen_Mid + Vector2(0, -100), Vector2(.5f, .5f));
	gameCanvas->AddUIObject(text);

	CurrentCanvas = gameCanvas;
}

void UISystem::Update(float deltaTime)
{
}

void UISystem::EnableCanvasByID(std::string id)
{
	for (auto& canvas : Canvases)
	{
		if (canvas->GetID() == id)
			EnableCanvas(canvas);
	}
}

void UISystem::EnableCanvas(CanvasUI* canvas)
{
	if (CurrentCanvas != nullptr)
	{
		CurrentCanvas->SetEnabled (false);
		LastActiveCanvas = CurrentCanvas;
	}
	CurrentCanvas = canvas;
	canvas->SetEnabled (true);
}

void UISystem::Clean()
{
	for (auto& canvas : Canvases)
	{
		delete canvas;
	}
	Canvases.clear();
}

CanvasUI* UISystem::GetCanvasUIByID(std::string id)
{
	for (auto& canvas : Canvases)
	{
		if (canvas->GetID() == id)
			return canvas;
	}
	return nullptr;
}
