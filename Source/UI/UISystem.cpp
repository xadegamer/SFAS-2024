#include "UISystem.h"

#include "Game/TypeHelpers.h"
#include "Engine/IGraphics.h"
#include "Engine/IInput.h"
#include "Game/SoundManager.h"

std::vector<CanvasUI*> UISystem::Canvases;
CanvasUI* UISystem::LastActiveCanvas = nullptr;
CanvasUI* UISystem::CurrentCanvas = nullptr;
IGraphics* UISystem::Graphics = nullptr;
IInput* UISystem::Input = nullptr;

const uint32_t kDisplayBufferWidth = 1920 / 1.2;
const uint32_t kDisplayBufferHeight = 1080 / 1.2;

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
	PauseMenu();
	GameOverMenu();
	WinMenu();
}

void UISystem::SetUpMainMenuCanvas()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* mainMenuCanvas = new CanvasUI(Graphics,"MainMenuCanvas");
	Canvases.push_back(mainMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/Background.dds");
	bg->SetScale(Vector2(1.0f, 1.0f));
	mainMenuCanvas->AddUIObject(bg);

	Text* text = new Text("Title Text", L"Water Works", Screen_Mid + Vector2(0, -200), Vector2(1.5, 1.5));
	text->SetColor(Color_White);
	mainMenuCanvas->AddUIObject(text);

	Text* audioVolumeText = new Text("VolumeText", L"Volume", Screen_Mid + Vector2(0, 300), Vector2(.5f, .5f));
	audioVolumeText->SetColor(Color_White);
	mainMenuCanvas->AddUIObject(audioVolumeText);

	Button* StartButton = new Button("Start_B", Graphics, Resolution_Mid + Vector2(0, 0), Vector2(.5, .5f));
	StartButton->AddText("Text", L"Start", Screen_Mid + Vector2(0, -10), Vector2(.5f, .5f), Color_White);
	StartButton->AddHighlightEventListener([]()
	{
			SoundManager::PlayOneShot("Button_Hover");
	});
	mainMenuCanvas->AddUIObject(StartButton);

	Button* QuitButton = new Button("Quit_B", Graphics, Resolution_Mid + Vector2(0,-100), Vector2(.5, .5f));
	QuitButton->AddText("Text", L"Quit", Screen_Mid + Vector2(0, 90), Vector2(.5f, .5f), Color_White);
	QuitButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	mainMenuCanvas->AddUIObject(QuitButton);

	ButtonNavigator* navigator = new ButtonNavigator(Input);
	navigator->AddButton(StartButton);
	navigator->AddButton(QuitButton);
	mainMenuCanvas->AddUIObject(navigator);

	Image* infoPanel = new Image("InfoPanel", Graphics, L"Resource/Textures/TopTextBox.dds", Vector2(0, 100), 15);
	infoPanel->SetScale(Vector2(.8f,.8f));
	mainMenuCanvas->AddUIObject(infoPanel);

	///////// Icon + 50, Text + 100

	Image* dPadIcon = new Image("dPadLeftIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Left.dds", Resolution_Mid + Vector2(-300, 420), 16, Vector2(.1f, .1f));
	mainMenuCanvas->AddUIObject(dPadIcon);

	Image* dPadRightIcon = new Image("dPadRightIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Right.dds", Resolution_Mid + Vector2(-250, 420), 16, Vector2(.1f, .1f));
	mainMenuCanvas->AddUIObject(dPadRightIcon);

	Text* dPadText = new Text("dPadLeftText", L"--- Navigate Volume", Screen_Mid + Vector2(-140, -420), Vector2(.35, .35));
	mainMenuCanvas->AddUIObject(dPadText);

	/////////

	Image* dPadUpIcon = new Image("dPadUpIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Up.dds", Resolution_Mid + Vector2(50, 420), 16, Vector2(.1f, .1f));
	mainMenuCanvas->AddUIObject(dPadUpIcon);

	Image* dPadDownIcon = new Image("dPadUpIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Down.dds", Resolution_Mid + Vector2(100, 420), 16, Vector2(.1f, .1f));
	mainMenuCanvas->AddUIObject(dPadDownIcon);

	Text* dPadUpText = new Text("dPadUpText", L"--- Navigate Menu", Screen_Mid + Vector2(200, -420), Vector2(.35, .35));
	mainMenuCanvas->AddUIObject(dPadUpText);

	/////////

	AddLeaderboardUI (mainMenuCanvas);

	AddHintUI (mainMenuCanvas);

	CurrentCanvas = mainMenuCanvas;
}

void UISystem::SetUpGameCanvas()
{
	CanvasUI* gameCanvas = new CanvasUI(Graphics,"GameCanvas");
	Canvases.push_back(gameCanvas);

	Image* infoPanel = new Image("InfoPanel", Graphics, L"Resource/Textures/TopTextBox.dds", Vector2(0, 100), 15);
	infoPanel->SetScale(Vector2(.8f, .8f));
	gameCanvas->AddUIObject(infoPanel);

	///////// Icon + 50, Text + 100
	Image* leftTriggerIcon = new Image("dPadLeftIcon", Graphics, L"Resource/Textures/Buttons/Left Trigger.dds", Resolution_Mid + Vector2(-420, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(leftTriggerIcon);

	Image* rightTriggerIcon = new Image("dPadRightIcon", Graphics, L"Resource/Textures/Buttons/Right Trigger.dds", Resolution_Mid + Vector2(-370, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(rightTriggerIcon);

	Text* rightTriggerText = new Text("dPadRightText", L"- Switch Segment", Screen_Mid + Vector2(-270, -420), Vector2(.35, .35));
	gameCanvas->AddUIObject(rightTriggerText);

	/////////
	Image* dPadIcon = new Image("dPadIcon", Graphics, L"Resource/Textures/Buttons/D-Pad.dds", Resolution_Mid + Vector2(-150, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(dPadIcon);

	Text* dPadDownText = new Text("dPadDownText", L"- Switch Ring", Screen_Mid + Vector2(-70, -420), Vector2(.35, .35));
	gameCanvas->AddUIObject(dPadDownText);

	/////////
	Image* rightStickIcon = new Image("RightStick", Graphics, L"Resource/Textures/Buttons/Right Stick Left-Right.dds", Resolution_Mid + Vector2(30, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(rightStickIcon);

	Text* rightStickIconText = new Text("dPadDownText", L"- Rotate", Screen_Mid + Vector2(100, -420), Vector2(.35, .35));
	gameCanvas->AddUIObject(rightStickIconText);

	/////////

	Image* aIcon = new Image("aIcon", Graphics, L"Resource/Textures/Buttons/A.dds", Resolution_Mid + Vector2(180, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(aIcon);

	Text* aText = new Text("aText", L"- Confirm", Screen_Mid + Vector2(250, -420), Vector2(.35, .35));
	gameCanvas->AddUIObject(aText);

	/////////

	Image* menuIcon = new Image("menuIcon", Graphics, L"Resource/Textures/Buttons/Menu.dds", Resolution_Mid + Vector2(340, 420), 16, Vector2(.1f, .1f));
	gameCanvas->AddUIObject(menuIcon);

	Text* menuText = new Text("aText", L"- Pause", Screen_Mid + Vector2(400, -420), Vector2(.35, .35));
	gameCanvas->AddUIObject(menuText);

	////////

	Image* clockIcon = new Image("clockIcon", Graphics, L"Resource/Textures/Clock.dds", Resolution_Mid + Vector2(-695, 340), 16, Vector2(.25f, .25f));
	gameCanvas->AddUIObject(clockIcon);

	Text* text = new Text("TimeText", L"", Screen_TopLeftCorner + Vector2(100, 100), Vector2(.5f, .5f), Color_Red);
	gameCanvas->AddUIObject(text);

	Text* debugText = new Text("DebugText", L"", Screen_Mid + Vector2(0, 400), Vector2(.5f, .5f));
	gameCanvas->AddUIObject(debugText);
}

void UISystem::PauseMenu()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* pauseMenuCanvas = new CanvasUI(Graphics, "PauseMenuCanvas");
	Canvases.push_back(pauseMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/Background.dds");
	bg->SetScale(Vector2(1.5f, 1.5f));
	pauseMenuCanvas->AddUIObject(bg);

	Text* text = new Text("Title Text", L"Paused", Screen_Mid + Vector2(0, -200), Vector2(1.0f, 1.0f));
	text->SetColor(Color_White);
	pauseMenuCanvas->AddUIObject(text);

	Button* resumeButton = new Button("Resume_B", Graphics, Resolution_Mid + Vector2(0, 0), Vector2(.5, .5f));
	resumeButton->AddText("Text", L"Resume", Screen_Mid + Vector2(0, -10), Vector2(.5f, .5f), Color_White);
	resumeButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	pauseMenuCanvas->AddUIObject(resumeButton);

	Button* restartButton = new Button("Restart_B", Graphics, Resolution_Mid + Vector2(0, -100), Vector2(.5, .5f));
	restartButton->AddText("Text", L"Restart", Screen_Mid + Vector2(0, 90), Vector2(.5f, .5f), Color_White);
	restartButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	pauseMenuCanvas->AddUIObject(restartButton);

	Button* quitButton = new Button("Quit_B", Graphics, Resolution_Mid + Vector2(0, -200), Vector2(.5, .5f));
	quitButton->AddText("Text", L"Quit", Screen_Mid + Vector2(0, 190), Vector2(.5f, .5f), Color_White);
	quitButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	pauseMenuCanvas->AddUIObject(quitButton);

	ButtonNavigator* navigator = new ButtonNavigator(Input);
	navigator->AddButton(resumeButton);
	navigator->AddButton(restartButton);
	navigator->AddButton(quitButton);
	pauseMenuCanvas->AddUIObject(navigator);

	Image* infoPanel = new Image("InfoPanel", Graphics, L"Resource/Textures/TopTextBox.dds", Vector2(0, 100), 15);
	infoPanel->SetScale(Vector2(.8f, .8f));
	pauseMenuCanvas->AddUIObject(infoPanel);

	///////// Icon + 50, Text + 100

	Image* dPadIcon = new Image("dPadLeftIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Left.dds", Resolution_Mid + Vector2(-300, 420), 16, Vector2(.1f, .1f));
	pauseMenuCanvas->AddUIObject(dPadIcon);

	Image* dPadRightIcon = new Image("dPadRightIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Right.dds", Resolution_Mid + Vector2(-250, 420), 16, Vector2(.1f, .1f));
	pauseMenuCanvas->AddUIObject(dPadRightIcon);

	Text* dPadText = new Text("dPadLeftText", L"--- Navigate Volume", Screen_Mid + Vector2(-140, -420), Vector2(.35, .35));
	pauseMenuCanvas->AddUIObject(dPadText);

	/////////

	Image* dPadUpIcon = new Image("dPadUpIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Up.dds", Resolution_Mid + Vector2(50, 420), 16, Vector2(.1f, .1f));
	pauseMenuCanvas->AddUIObject(dPadUpIcon);

	Image* dPadDownIcon = new Image("dPadUpIcon", Graphics, L"Resource/Textures/Buttons/D-Pad Down.dds", Resolution_Mid + Vector2(100, 420), 16, Vector2(.1f, .1f));
	pauseMenuCanvas->AddUIObject(dPadDownIcon);

	Text* dPadUpText = new Text("dPadUpText", L"--- Navigate Menu", Screen_Mid + Vector2(200, -420), Vector2(.35, .35));
	pauseMenuCanvas->AddUIObject(dPadUpText);

	Text* audioVolumeText = new Text("VolumeText", L"Volume", Screen_Mid + Vector2(0, 300), Vector2(.5f, .5f));
	audioVolumeText->SetColor(Color_White);
	pauseMenuCanvas->AddUIObject(audioVolumeText);

	AddInstructionsUI (pauseMenuCanvas);

	AddHintUI(pauseMenuCanvas);
}

void UISystem::GameOverMenu()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* gameOverMenuCanvas = new CanvasUI(Graphics, "GameOverCanvas");
	Canvases.push_back(gameOverMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/Background.dds");
	bg->SetScale(Vector2(1.5f, 1.5f));
	gameOverMenuCanvas->AddUIObject(bg);

	Text* text = new Text("Title Text", L"GameOver", Screen_Mid + Vector2(0, -350), Vector2(1.5f, 1.5f));
	text->SetColor(Color_White);
	gameOverMenuCanvas->AddUIObject(text);

	Button* resumeButton = new Button("Restart_B", Graphics, Resolution_Mid + Vector2(0, 0), Vector2(.5, .5f));
	resumeButton->AddText("Text", L"Restart", Screen_Mid + Vector2(0, -10), Vector2(.5f, .5f), Color_White);
	resumeButton->AddHighlightEventListener([]()
	{
		SoundManager::PlayOneShot("Button_Hover");
	});
	gameOverMenuCanvas->AddUIObject(resumeButton);

	Button* quitButton = new Button("Quit_B", Graphics, Resolution_Mid + Vector2(0, -100), Vector2(.5, .5f));
	quitButton->AddText("Text", L"Quit", Screen_Mid + Vector2(0, 90), Vector2(.5f, .5f), Color_White);
	quitButton->AddHighlightEventListener([]()
	{
		SoundManager::PlayOneShot("Button_Hover");
	});
	gameOverMenuCanvas->AddUIObject(quitButton);

	ButtonNavigator* navigator = new ButtonNavigator(Input);
	navigator->AddButton(resumeButton);
	navigator->AddButton(quitButton);
	gameOverMenuCanvas->AddUIObject(navigator);


	Image* star1BG = new Image("Star1BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(-200, 200), 16, Vector2(.25f, .25f));
	gameOverMenuCanvas->AddUIObject(star1BG);

	Image* star2BG = new Image("Star2BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(0, 200), 16, Vector2(.5f, .5f));
	gameOverMenuCanvas->AddUIObject(star2BG);

	Image* star3BG = new Image("Star3BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(200, 200), 16, Vector2(.25f, .25f));
	gameOverMenuCanvas->AddUIObject(star3BG);

	/////////

	AddLeaderboardUI (gameOverMenuCanvas);
}

void UISystem::WinMenu()
{
	float aspectRatio = (float)kDisplayBufferWidth / (float)kDisplayBufferHeight;

	CanvasUI* winMenuCanvas = new CanvasUI(Graphics, "WinMenuCanvas");
	Canvases.push_back(winMenuCanvas);

	Image* bg = new Image("BG", Graphics, L"Resource/Textures/Background.dds");
	bg->SetScale(Vector2(1.5f, 1.5f));
	winMenuCanvas->AddUIObject(bg);

	Text* text = new Text("Title Text", L"You Win", Screen_Mid + Vector2(0, -350), Vector2(1.5f, 1.5f));
	text->SetColor(Color_White);
	winMenuCanvas->AddUIObject(text);

	Button* resumeButton = new Button("Restart_B", Graphics, Resolution_Mid + Vector2(0, 0), Vector2(.5, .5f));
	resumeButton->AddText("Text", L"Restart", Screen_Mid + Vector2(0, -10), Vector2(.5f, .5f), Color_White);
	resumeButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	winMenuCanvas->AddUIObject(resumeButton);

	Button* quitButton = new Button("Quit_B", Graphics, Resolution_Mid + Vector2(0, -100), Vector2(.5, .5f));
	quitButton->AddText("Text", L"Quit", Screen_Mid + Vector2(0, 90), Vector2(.5f, .5f), Color_White);
	quitButton->AddHighlightEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Hover");
		});
	winMenuCanvas->AddUIObject(quitButton);

	ButtonNavigator* navigator = new ButtonNavigator(Input);
	navigator->AddButton(resumeButton);
	navigator->AddButton(quitButton);
	winMenuCanvas->AddUIObject(navigator);


	Image* star1BG = new Image("Star1BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(-200, 200), 16, Vector2(.25f, .25f));
	winMenuCanvas->AddUIObject(star1BG);

	Image* star2BG = new Image("Star2BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(0, 200), 16, Vector2(.5f, .5f));
	winMenuCanvas->AddUIObject(star2BG);

	Image* star3BG = new Image("Star3BG", Graphics, L"Resource/Textures/Stars/StarBG.dds", Resolution_Mid + Vector2(200, 200), 16, Vector2(.25f, .25f));
	winMenuCanvas->AddUIObject(star3BG);

	Image* star1 = new Image("Star1", Graphics, L"Resource/Textures/Stars/Star.dds", Resolution_Mid + Vector2(-200, 200), 17, Vector2(.25f, .25f));
	winMenuCanvas->AddUIObject(star1);

	Image* star2 = new Image("Star2", Graphics, L"Resource/Textures/Stars/Star.dds", Resolution_Mid + Vector2(0, 200), 17, Vector2(.5f, .5f));
	winMenuCanvas->AddUIObject(star2);

	Image* star3 = new Image("Star3", Graphics, L"Resource/Textures/Stars/Star.dds", Resolution_Mid + Vector2(200, 200), 17, Vector2(.25f, .25f));
	winMenuCanvas->AddUIObject(star3);

	Text* timetext = new Text("TimeText", L"", Screen_Mid + Vector2(0, -90), Vector2(.75f, .75f));
	winMenuCanvas->AddUIObject(timetext);

	/////////

	AddLeaderboardUI(winMenuCanvas);

}

void UISystem::AddLeaderboardUI(CanvasUI* canvas)
{
	Image* leaderboardBG = new Image("leaderboardBG", Graphics, L"Resource/Textures/TextBoxBig.dds", Resolution_Mid + Vector2(500, -300), 16, Vector2(.8f, .8f));
	canvas->AddUIObject(leaderboardBG);

	Text* leaderboardHeaderText = new Text("leaderboardHeaderText", L"LeaderBoard", Screen_BottomRightCorner + Vector2(-290, -330), Vector2(1, 1), Color_White);
	canvas->AddUIObject(leaderboardHeaderText);

	Text* leaderboardText = new Text("leaderboardText", L"", Screen_BottomRightCorner + Vector2(-290, -160), Vector2(.5, .5));
	canvas->AddUIObject(leaderboardText);
}

void UISystem::AddInstructionsUI(CanvasUI* canvas)
{
	Text* instuctionHeaderText = new Text("instuctionHeaderText", L"Instructions", Screen_BottomLeftCorner + Vector2(190, -200), Vector2(1, 1), Color_White);
	canvas->AddUIObject(instuctionHeaderText);

	std::wstring infoTextString =

		L"Rotate the pipes to connect the water. \n\n" \
		L"Beat the time and get 1 - 3 Stars \n\n" \
;
	Text* instuctionText = new Text("instuctionText", infoTextString, Screen_BottomLeftCorner + Vector2(290, -100), Vector2(.5, .5));
	canvas->AddUIObject(instuctionText);
}

void UISystem::AddHintUI(CanvasUI* canvas)
{
	Image* hintIcon = new Image("hintBG", Graphics, L"Resource/Textures/Buttons/Y.dds", Resolution_Mid + Vector2(-700, 300), 16, Vector2(.1f, .1f));
	canvas->AddUIObject(hintIcon);

	Text* hintHeaderText = new Text("hintText", L"Hint Off", Screen_TopLeftCorner + Vector2(100, 200), Vector2(.5, .5), Color_Red);
	canvas->AddUIObject(hintHeaderText);
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
