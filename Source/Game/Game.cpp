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

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), State()
{

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
	StartGame();
	State = GameState::Playing;
	return true;
}

void Game::Update()
{
	switch (State)
	{
	case Setup:

		break;
	case Playing:
		UpdateRingSelection();
		UpdateSelectedRingRotation();
		break;
	default:	break;
	}
}

void Game::Cleanup()
{

}

void Game::StartGame()
{
	ITexture* BGTexture = Graphics->CreateTexture(L"Resource/Textures/BG_Gray.dds");
	IShader* BGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", BGTexture);
	CentrebGG = Graphics->CreateBillboard(BGShader,0);

	// left Ttop
	RingHolder* TestRingHolder0 = new RingHolder(Graphics , L"TopLeft");
	TestRingHolder0->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder0->SetPosition(-100, 100);
	TestRingHolder0->SetScale(0.5f, 0.5f);

	// left buttom
	RingHolder* TestRingHolder1 = new RingHolder(Graphics, L"BottomLeft");
	TestRingHolder1->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder1->SetPosition(-100, -100);
	TestRingHolder1->SetScale(0.5f, 0.5f);

	// right top
	RingHolder* TestRingHolder2 = new RingHolder(Graphics , L"TopRight");
	TestRingHolder2->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder2->SetPosition(100, 100);
	TestRingHolder2->SetScale(0.5f, 0.5f);

	// right buttom
	RingHolder* TestRingHolder3 = new RingHolder(Graphics , L"BottomRight");
	TestRingHolder3->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder3->SetPosition(100, -100);
	TestRingHolder3->SetScale(0.5f, 0.5f);

	RingHolders.push_back(TestRingHolder0);
	RingHolders.push_back(TestRingHolder1);
	RingHolders.push_back(TestRingHolder2);
	RingHolders.push_back(TestRingHolder3);

	RingHolders[CurrentRingHolderIndex]->Activate();

	WaterTank1 = new WaterTank(Graphics);
	WaterTank1->SetPosition(-300, 0);
	WaterTank1->SetOnEmptyEvent(std::bind(&Game::OnFailure, this));

	WaterTank2 = new WaterTank(Graphics);
	WaterTank2->SetPosition(300, 0);
	WaterTank2->SetWaterLevel(-200);

	State = GameState::Setup;

	SoundManager::StopMusic("MainMenu");
	SoundManager::PlayMusic("GameMenu");

	UISystem::EnableCanvasByID("GameCanvas");

	// If mode is Setup game then set each ring to a random rotation
	SetupEachRing();
}

void Game::SetupEachRing()
{
	for (size_t i = 0; i < RingHolders.size(); i++)
	{
		RingHolders[i]->SetupRings();
	}
}

void Game::UpdateRingSelection()
{
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

		int random = rand() % 100;
		std::wstring str = std::to_wstring(random);
		//UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("ScoreText")->SetText(str);
	}
	else if (Input->IsPressed(InputAction::DirectionPadBottom))
	{
		SwitchToNextRingHolder(1);
	}

	if (Input->IsPressed(InputAction::DefaultSelect))
	{
		RingHolders[CurrentRingHolderIndex]->CheckForSuccess();
	}

	WaterTank1->Update();
	WaterTank2->Update();

	TransferWater();

	UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText")->SetText(
		std::to_wstring(RingHolders[CurrentRingHolderIndex]->GetSelectedRingRotation()));
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
}

void Game::OnFailure()
{
	IsConnected = false;
}

void Game::TransferWater()
{
	WaterTank1->UpdateWaterLevel(-1);

	if (IsConnected)
	WaterTank2->UpdateWaterLevel(1);
}