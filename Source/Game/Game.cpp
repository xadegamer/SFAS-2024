#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"

#include "RingHolder.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = Pie / 90.0f;

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
	CurrentRingHolder = new RingHolder(Graphics);	
	CurrentRingHolder->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	CurrentRingHolder->SetPosition(0, 0);
	CurrentRingHolder->SetScale(0.5f, 0.5f);

	TestRingHolder = new RingHolder(Graphics);
	TestRingHolder->AddSuccessEventListener(std::bind(&Game::OnSuccess, this));
	TestRingHolder->SetPosition(300, 0);
	TestRingHolder->SetScale(0.5f, 0.5f);

	State = GameState::Setup;

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
}

void Game::Cleanup()
{

}

void Game::SetupEachRing()
{
	CurrentRingHolder->SetupRings();
}

void Game::UpdateRingSelection()
{
	if (Input->IsPressed(InputAction::ShoulderButtonLeft))
	{
		// Change ring selection towards outer
		CurrentRingHolder->UpdateRingSelection(-1);
	}
	else if (Input->IsPressed(InputAction::ShoulderButtonRight))
	{
		// Change ring selection towards inner
		CurrentRingHolder->UpdateRingSelection(1);
	}

	if (Input->IsPressed(InputAction::DirectionPadTop))
	{
		// X on controller
		CurrentRingHolder->UpdateRingSelection(-1);
	}
}

void Game::UpdateSelectedRingRotation()
{
	CurrentRingHolder->UpdateSelectedRingRotation(Input->GetValue(InputAction::RightStickXAxis));
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
	CurrentRingHolder->CheckForSuccess();
}

void Game::OnSuccess()
{
	IsValid();
}
