#include "UISystem.h"
#include "CanvasUI.h"
#include "UIObject.h"
#include "Text.h"

std::vector<CanvasUI*> UISystem::Canvases;
CanvasUI* UISystem::LastActiveCanvas = nullptr;
CanvasUI* UISystem::CurrentCanvas = nullptr;

static const uint32_t kDisplayBufferWidth = 1920 / 2;
static const uint32_t kDisplayBufferHeight = 1080 / 2;

void UISystem::Init()
{
	SetUpGameCanvas();
}

void UISystem::SetUpGameCanvas()
{
	CanvasUI* gameCanvas = new CanvasUI("GameCanvas", true);
	Canvases.push_back(gameCanvas);
	Text* text = new Text("ScoreText", L"Score: 0", Vector2(0.0f, 0.0f), Vector2(0.5f, 0.5f));
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
