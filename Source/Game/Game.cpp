#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include "RingHolder.h"
#include "WaterTank.h"

#include "SoundManager.h"
#include "UI/UISystem.h"

#include "Engine/ParticleSystem.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)
const std::string MainMenuCanvasID = "MainMenuCanvas";
const std::string PauseMenuCanvasID = "PauseMenuCanvas";

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), State(), RingHolders(), CurrentRingHolderIndex(0), CentrebGG(), WaterTank1(), WaterTank2(), IsPaused(false), IsConnected(false)
{
	Button* StartButton = UISystem::GetCanvasUIByID(MainMenuCanvasID)->GetUIObjectByID<Button>("Start_B");
	if (StartButton)
	{
		StartButton->AddSelectEventListener([this]()
		{
			SoundManager::PlayOneShot("Button_Click");
			Load();
		});
	}

	Button* QuitButton = UISystem::GetCanvasUIByID(MainMenuCanvasID)->GetUIObjectByID<Button>("Quit_B");
	if (QuitButton)
	{
		QuitButton->AddSelectEventListener([]()
		{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	Button* ResumeButton = UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Button>("Resume_B");
	if (ResumeButton)
	{
		ResumeButton->AddSelectEventListener([this]()
		{
			SoundManager::PlayOneShot("Button_Click");
			SetIsPaused(false);
		});
	}

	Button* QuitButton2 = UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Button>("Quit_B");
	if (QuitButton2)
	{
		QuitButton2->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	Button* RestartButton = UISystem::GetCanvasUIByID("GameOverCanvas")->GetUIObjectByID<Button>("Restart_B");
	if (RestartButton)
	{
		RestartButton->AddSelectEventListener([this]()
			{
			SoundManager::PlayOneShot("Button_Click");
			StartGame();
		});
	}

	Button* QuitButton3 = UISystem::GetCanvasUIByID("GameOverCanvas")->GetUIObjectByID<Button>("Quit_B");
	if (QuitButton3)
	{
		QuitButton3->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	Button* RestartButton2 = UISystem::GetCanvasUIByID("WinMenuCanvas")->GetUIObjectByID<Button>("Restart_B");
	if (RestartButton2)
	{
		RestartButton2->AddSelectEventListener([this]()
			{
			SoundManager::PlayOneShot("Button_Click");
			StartGame();
		});
	}

	Button* QuitButton4 = UISystem::GetCanvasUIByID("WinMenuCanvas")->GetUIObjectByID<Button>("Quit_B");
	if (QuitButton4)
	{
		QuitButton4->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	UISystem::EnableCanvasByID(MainMenuCanvasID);
}

Game::~Game()
{
}

bool Game::IsValid()
{
	return true;
}

bool Game::Load()
{
	SetUpGame();
	StartGame();
	return true;
}

void Game::Update()
{
	switch (State)
	{
		case Setup:

			break;
		case Playing:
			if (IsPaused) return;
			UpdateRingSelection();
			UpdateSelectedRingRotation();
			break;
		default: break;
	}
}

void Game::Cleanup()
{

}

void Game::SetUpGame()
{
	ITexture* BGTexture = Graphics->CreateTexture(L"Resource/Textures/BG_Gray.dds");
	IShader* BGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", BGTexture);
	CentrebGG = Graphics->CreateBillboard(BGShader,0);
	CentrebGG->SetScale(1.5f, 1.5f);

	// left Ttop
	RingHolder* TestRingHolder0 = new RingHolder(Graphics , L"TopLeft");
	TestRingHolder0->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder0->SetPosition(Vector2(-100, 100));
	TestRingHolder0->SetScale(Vector2(0.5f, 0.5f));

	// left buttom
	RingHolder* TestRingHolder1 = new RingHolder(Graphics, L"BottomLeft");
	TestRingHolder1->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder1->SetPosition(Vector2 (-100, -100));
	TestRingHolder1->SetScale(Vector2(0.5f, 0.5f));

	// right top
	RingHolder* TestRingHolder2 = new RingHolder(Graphics , L"TopRight");
	TestRingHolder2->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder2->SetPosition(Vector2(100, 100));
	TestRingHolder2->SetScale(Vector2(0.5f, 0.5f));

	// right buttom
	RingHolder* TestRingHolder3 = new RingHolder(Graphics , L"BottomRight");
	TestRingHolder3->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder3->SetPosition(Vector2(100, -100));
	TestRingHolder3->SetScale(Vector2(0.5f, 0.5f));

	RingHolders.push_back(TestRingHolder0);
	RingHolders.push_back(TestRingHolder1);
	RingHolders.push_back(TestRingHolder2);
	RingHolders.push_back(TestRingHolder3);

	RingHolders[CurrentRingHolderIndex]->Activate();

	WaterTank1 = new WaterTank(Graphics);
	WaterTank1->SetPosition(Vector2 (-300, 0) );
	WaterTank1->SetOnEmptyEvent(std::bind(&Game::OnFirstTankEmpty, this));

	WaterTank2 = new WaterTank(Graphics);
	WaterTank2->SetPosition(Vector2(300, 0));

	State = GameState::Setup;

	SoundManager::PlayMusic("GameMenu");
	SoundManager::SetVolume("GameMenu", 0.2f);
}

void Game::StartGame()
{
	SetupEachRing();
	WaterTank1->SetWaterLevel(0);
	WaterTank1->Reset();
	WaterTank2->SetWaterLevel(-200);
	WaterTank2->Reset();
	UISystem::EnableCanvasByID("GameCanvas");
	State = GameState::Playing;
}

void Game::SetupEachRing()
{
	for (size_t i = 0; i < RingHolders.size(); i++)
	{
		RingHolders[i]->SetupRings();
	}

	RingHolders[CurrentRingHolderIndex]->Activate();
}

void Game::UpdateRingSelection()
{
	if (Input->IsPressed(InputAction::SpecialLeft))
	{
		SetIsPaused(true);
		return;
	}
	
	if (Input->IsPressed(InputAction::ShoulderButtonLeft))
	{
		// Change ring selection towards outer
		RingHolders[CurrentRingHolderIndex]->UpdateRingSelection(-1);
	}
	else if (Input->IsPressed(InputAction::ShoulderButtonRight))
	{
		// Change ring selection towards inner
		RingHolders[CurrentRingHolderIndex]->UpdateRingSelection(1);
	}

	if (Input->IsPressed(InputAction::DirectionPadTop))
	{
		SwitchToNextRingHolder(-1);
	}
	else if (Input->IsPressed(InputAction::DirectionPadBottom))
	{
		SwitchToNextRingHolder(1);
	}

	if (Input->IsPressed(InputAction::ButtonBottom))
	{
		RingHolders[CurrentRingHolderIndex]->CheckForSuccess();
	}

	WaterTank1->Update();
	WaterTank2->Update();

	TransferWater();

	Text* debugText = UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText");

	if (debugText)
	{
		/*	int random = rand() % 100;
		std::wstring str = std::to_wstring(random);*/

		//UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("ScoreText")->SetText(str);
		//debugText->SetText(std::to_wstring(RingHolders[CurrentRingHolderIndex]->GetSelectedRingRotation()));
		debugText->SetText(std::to_wstring(RingHolders[CurrentRingHolderIndex]->ValidateRings()));
		//debugText->SetText(std::to_wstring(WaterTank1->GetNormalizedWaterLevel()));
	}
}

void Game::UpdateSelectedRingRotation()
{
	RingHolders[CurrentRingHolderIndex]->UpdateSelectedRingRotation(Input->GetValue(InputAction::RightStickXAxis));
}

void Game::SwitchToNextRingHolder(int direction)
{
	int currentRingHolderIndex = CurrentRingHolderIndex;
	int nextRingHolderIndex = CLAMP(currentRingHolderIndex + direction, 0, RingHolders.size() - 1);
	if (nextRingHolderIndex == currentRingHolderIndex) return;

	RingHolders[CurrentRingHolderIndex]->Deactivate();
	CurrentRingHolderIndex = nextRingHolderIndex;
	RingHolders[CurrentRingHolderIndex]->Activate();
}

void Game::OnSuccess()
{
	IsConnected = true;
	WaterTank1->UpdateWaterLevel(1);
}

void Game::OnFirstTankEmpty()
{
	IsConnected = false;

	if (WaterTank2->GetNormalizedWaterLevel() == 0)
	{
		State = GameState::GameOver;
		UISystem::EnableCanvasByID("GameOverCanvas");
	}
	else
	{
		State = GameState::Win;
		UISystem::EnableCanvasByID("WinMenuCanvas");
	}
}

void Game::TransferWater()
{
	WaterTank1->UpdateWaterLevel(-1);

	if (IsConnected)
	WaterTank2->UpdateWaterLevel(1);
}

void Game::SetIsPaused(bool isPaused)
{
	IsPaused = isPaused;

	if (IsPaused)
	{
		UISystem::EnableCanvasByID("PauseMenuCanvas");
	}
	else
	{
		UISystem::EnableCanvasByID("GameCanvas");
	}
}
