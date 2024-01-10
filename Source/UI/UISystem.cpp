#include "UISystem.h"


#include "Game/TypeHelpers.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"

std::vector<CanvasUI*> UISystem::Canvases;
CanvasUI* UISystem::LastActiveCanvas = nullptr;
CanvasUI* UISystem::CurrentCanvas = nullptr;
IGraphics* UISystem::Graphics = nullptr;
IInput* UISystem::Input = nullptr;

const uint32_t kDisplayBufferWidth = 1920 / 2;
const uint32_t kDisplayBufferHeight = 1080 / 2;

const Vector2 Screen_Mid = Vector2(kDisplayBufferWidth / 2, kDisplayBufferHeight / 2);
const Vector2 Screen_TopRightCorner = Vector2(kDisplayBufferWidth, 0);
const Vector2 Screen_TopLeftCorner = Vector2(0, 0);
const Vector2 Screen_BottomRightCorner = Vector2(kDisplayBufferWidth, kDisplayBufferHeight);
const Vector2 Screen_BottomLeftCorner = Vector2(0, kDisplayBufferHeight);

const Vector2 Resolution_Mid = Vector2(kDisplayBufferWidth / kDisplayBufferHeight, kDisplayBufferWidth / kDisplayBufferHeight);

const Vector4 Color_White = Vector4(1, 1, 1, 1);
const Vector4 Color_Black = Vector4(0, 0, 0, 1);
const Vector4 Color_Red = Vector4(1, 0, 0, 1);
const Vector4 Color_Green = Vector4(0, 1, 0, 1);
const Vector4 Color_Blue = Vector4(0, 0, 1, 1);

void UISystem::Init(IGraphics* graphics, IInput* input)
{
	Graphics = graphics;
	Input = input;

	SetUpMainMenuCanvas();
	SetUpGameCanvas();
}

void UISystem::SetUpMainMenuCanvas()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* mainMenuCanvas = new CanvasUI(Graphics,"MainMenuCanvas", true);
	Canvases.push_back(mainMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/BG_DeepBlue.dds");
	mainMenuCanvas->AddUIObject(bg);

	Text* text = new Text("Title Text", L"Good Game", Screen_Mid + Vector2(0, -200), Vector2(.5f, .5f));
	text->SetColor(Color_White);
	mainMenuCanvas->AddUIObject(text);

	Button* StartButton = new Button("Start_B", Graphics, Resolution_Mid + Vector2(0, 100), Vector2(2.5f, .5f));
	StartButton->AddText("Text", L"Start", Screen_Mid + Vector2(0, -110), Vector2(.5f, .5f), Color_Red);
	mainMenuCanvas->AddUIObject(StartButton);

	Button* QuitButton = new Button("Quit_B", Graphics, Resolution_Mid + Vector2(0,-10), Vector2(2.5f, .5f));
	QuitButton->AddText("Text", L"Quit", Screen_Mid + Vector2(0, 5), Vector2(.5f, .5f), Color_Red);
	mainMenuCanvas->AddUIObject(QuitButton);

	ButtonNavigator* navigator = new ButtonNavigator(Input);
	navigator->AddButton(StartButton);
	navigator->AddButton(QuitButton);
	mainMenuCanvas->AddUIObject(navigator);

	CurrentCanvas = mainMenuCanvas;
}

void UISystem::SetUpGameCanvas()
{
	CanvasUI* gameCanvas = new CanvasUI(Graphics,"GameCanvas");
	Canvases.push_back(gameCanvas);

	Text* text = new Text("ScoreText", L"Score", Screen_TopLeftCorner + Vector2(30,30), Vector2(.5f, .5f));
	gameCanvas->AddUIObject(text);

	Text* debugText = new Text("DebugText", L"Debug", Screen_Mid + Vector2(0, 200), Vector2(.5f, .5f));
	gameCanvas->AddUIObject(debugText);
}

void UISystem::Update(float deltaTime)
{
	for (auto& canvas : Canvases)
	{
		canvas->Update(deltaTime);
	}
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
