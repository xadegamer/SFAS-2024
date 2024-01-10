#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include "RingHolder.h"
#include "SoundManager.h"
#include "UI/UISystem.h"
#include "UI/CanvasUI.h"
#include "UI/Text.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), State()
{
	SoundManager::Initialize();
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
	// left top
	RingHolder* TestRingHolder0 = new RingHolder(Graphics);
	TestRingHolder0->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder0->SetPosition(-100,100);
	TestRingHolder0->SetScale(0.5f, 0.5f);

	// left buttom
	RingHolder* TestRingHolder1 = new RingHolder(Graphics);
	TestRingHolder1->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder1->SetPosition(-100,-100);
	TestRingHolder1->SetScale(0.5f, 0.5f);

	// right top
	RingHolder* TestRingHolder2 = new RingHolder(Graphics);
	TestRingHolder2->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder2->SetPosition(100, 100);
	TestRingHolder2->SetScale(0.5f, 0.5f);

	// right buttom
	RingHolder* TestRingHolder3 = new RingHolder(Graphics);
	TestRingHolder3->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder3->SetPosition(100, -100);
	TestRingHolder3->SetScale(0.5f, 0.5f);

	RingHolders.push_back(TestRingHolder0);
	RingHolders.push_back(TestRingHolder1);
	RingHolders.push_back(TestRingHolder2);
	RingHolders.push_back(TestRingHolder3);

	RingHolders[CurrentRingHolderIndex]->Activate();

	State = GameState::Setup;

	SoundManager::PlayMusic("MainTheme");

	ITexture* BGTexture = Graphics->CreateTexture(L"Resource/Textures/BG.dds");
	IShader* BGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", BGTexture);
	CentrebGG = Graphics->CreateBillboard(BGShader,0);

	return true;
}

void Game::Update()
{
	// If mode is Setup game then set each ring to a random rotation
	if (State == GameState::Setup)
	{
		SetupEachRing();
		State = GameState::Playing;
	}

	// If mode is Playing then read controller input and manage which ring is selected, the rotation of each ring and waiting for select to confirm positions 
	if (State == GameState::Playing)
	{
		UpdateRingSelection();
		UpdateSelectedRingRotation();
		UpdateRingTestSelection();
	}

	// If mode is Test then check to see if the rings are in their correct positions, play a noise corresponding to how close the player is 
	if (State == GameState::Test)
	{
		TestRingSolution();
		//State = GameState::Setup;
	}

	SoundManager::Update();
}

void Game::Cleanup()
{
	SoundManager::Clear();
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
		UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("ScoreText")->SetText(str);
	}
	else if (Input->IsPressed(InputAction::DirectionPadBottom))
	{
		SwitchToNextRingHolder(1);
	}
}

void Game::UpdateSelectedRingRotation()
{
	RingHolders[CurrentRingHolderIndex]->UpdateSelectedRingRotation(Input->GetValue(InputAction::RightStickXAxis));
}

void Game::UpdateRingTestSelection()
{
	if (Input->IsPressed(InputAction::DefaultSelect))
	{
		TestRingSolution();
		//State = GameState::Test;
	}
}

void Game::TestRingSolution()
{
	RingHolders[CurrentRingHolderIndex]->CheckForSuccess();
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
	IsValid();
}
