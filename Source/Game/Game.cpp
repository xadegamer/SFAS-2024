#include "Game.h"

#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"
#include "Engine/IInput.h"
#include "Engine/Time.h"
#include "Engine/ObjectSerializer.h"

#include "RingHolder.h"
#include "WaterTank.h"

#include "SoundManager.h"
#include "UI/UISystem.h"
#include "PlayerData.h"

#include "Engine/ParticleSystem.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)
const std::string MainMenuCanvasID = "MainMenuCanvas";
const std::string PauseMenuCanvasID = "PauseMenuCanvas";
const std::string GameCanvasID = "GameCanvas";
const std::string GameOverCanvasID = "GameOverCanvas";
const std::string WinMenuCanvasID = "WinMenuCanvas";
const std::string PlayerDataFileName = "PlayerData";

IApplication* GetApplication(IGraphics* Graphics, IInput* Input)
{
	return new Game(Graphics, Input);
}

Game::Game(IGraphics* GraphicsIn, IInput* InputIn) : IApplication(GraphicsIn, InputIn), State(), RingHolderGrid(), CentrebGG(), WaterTank1(), WaterTank2(), IsConnected(false)
{
	LoadPlayerData();

	WaterTransferSpeed = 20.0f;
	WaterLickSpeed = 1.0f;

	Star1Threshold = 0.25f;
	Star2Threshold = 0.5f;
	Star3Threshold = 0.75f;

	currentSessionTimerInterval = 1.0f;

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

	Button* pauseResumeButton = UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Button>("Resume_B");
	if (pauseResumeButton)
	{
		pauseResumeButton->AddSelectEventListener([this]()
		{
			SoundManager::PlayOneShot("Button_Click");
			TogglePause();
		});
	}

	Button* pauseRestartButton = UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Button>("Restart_B");
	if (pauseRestartButton)
	{
		pauseRestartButton->AddSelectEventListener([this]()
			{
				SoundManager::PlayOneShot("Button_Click");
				StartGame();
			});
	}

	Button* pasueQuitButton = UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Button>("Quit_B");
	if (pasueQuitButton)
	{
		pasueQuitButton->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	Button* gameoverRestartButton = UISystem::GetCanvasUIByID(GameOverCanvasID)->GetUIObjectByID<Button>("Restart_B");
	if (gameoverRestartButton)
	{
		gameoverRestartButton->AddSelectEventListener([this]()
			{
			SoundManager::PlayOneShot("Button_Click");
			StartGame();
		});
	}

	Button* gameoverQuitButton = UISystem::GetCanvasUIByID(GameOverCanvasID)->GetUIObjectByID<Button>("Quit_B");
	if (gameoverQuitButton)
	{
		gameoverQuitButton->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	Button* winRestartButton = UISystem::GetCanvasUIByID(WinMenuCanvasID)->GetUIObjectByID<Button>("Restart_B");
	if (winRestartButton)
	{
		winRestartButton->AddSelectEventListener([this]()
			{
			SoundManager::PlayOneShot("Button_Click");
			StartGame();
		});
	}

	Button* winQuitButton = UISystem::GetCanvasUIByID(WinMenuCanvasID)->GetUIObjectByID<Button>("Quit_B");
	if (winQuitButton)
	{
		winQuitButton->AddSelectEventListener([]()
			{
			SoundManager::PlayOneShot("Button_Click");
			PostQuitMessage(0);
		});
	}

	UISystem::EnableCanvasByID(MainMenuCanvasID);
	UISystem::GetCanvasUIByID(MainMenuCanvasID)->GetUIObjectByID<Text>("leaderboardText")->SetText(GetLeaderboardInfo());

	SetMusicVolume(MusicVolume);
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
			HandleVolumeChange();
			HandleHintInput();
			break;
		case Playing:
			HandGameTime();
			HandlePauseInput();
			UpdateRingSelection();
			UpdateSelectedRingRotation();
			UpdateTanks();
			HandleHint();
			break;
		case Paused:
			HandlePauseInput();
			HandleVolumeChange();
			HandleHintInput();
			break;
		default: break;
	}
}

void Game::Cleanup()
{
	for (size_t i = 0; i < RingHolderGrid.size(); i++)
	{
		for (size_t j = 0; j < RingHolderGrid[i].size(); j++)
		{
			delete RingHolderGrid[i][j];
		}
		RingHolderGrid[i].clear();
	}

	RingHolderGrid.clear();

	delete WaterTank1;
	delete WaterTank2;

	delete PlayerDataInstance;
}

void Game::SetUpGame()
{
	ITexture* BGTexture = Graphics->CreateTexture(L"Resource/Textures/Background.dds");
	IShader* BGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", BGTexture);
	CentrebGG = Graphics->CreateBillboard(BGShader,0);
	CentrebGG->SetScale(1.0f, 1.0f);

	// left Ttop
	RingHolder* TestRingHolder0 = new RingHolder(Graphics , L"TopLeft");
	TestRingHolder0->SetPosition(Vector2(-150, 150));
	TestRingHolder0->SetScale(Vector2(0.5f, 0.5f));

	// left buttom
	RingHolder* TestRingHolder1 = new RingHolder(Graphics, L"BottomLeft");
	TestRingHolder1->SetPosition(Vector2 (-148, -144));
	TestRingHolder1->SetScale(Vector2(0.5f, 0.5f));

	// right top
	RingHolder* TestRingHolder2 = new RingHolder(Graphics , L"TopRight");
	TestRingHolder2->SetPosition(Vector2(146.7, 147));
	TestRingHolder2->SetScale(Vector2(0.5f, 0.5f));

	// right buttom
	RingHolder* TestRingHolder3 = new RingHolder(Graphics , L"BottomRight");
	TestRingHolder3->SetPosition(Vector2(150, -147));
	TestRingHolder3->SetScale(Vector2(0.5f, 0.5f));

	RingHolderGrid.push_back(std::vector<RingHolder*>());
	RingHolderGrid.push_back(std::vector<RingHolder*>());

	RingHolderGrid[0].push_back(TestRingHolder0);
	RingHolderGrid[0].push_back(TestRingHolder1);

	RingHolderGrid[1].push_back(TestRingHolder2);
	RingHolderGrid[1].push_back(TestRingHolder3);

	SelectedRingHolder = RingHolderGrid[currentRow][ currentCol];
	SelectedRingHolder->Activate();

	WaterTank1 = new WaterTank(Graphics, true);
	WaterTank1->SetPosition(Vector2 (-495, 17) );
	WaterTank1->SetOnEmptyEvent(std::bind(&Game::OnFirstTankEmpty, this));

	WaterTank2 = new WaterTank(Graphics,false);
	WaterTank2->SetMarkerPositions(Star1Threshold, Star2Threshold, Star3Threshold);
	WaterTank2->SetPosition(Vector2(498, 17));

	ITexture* TankLowerMaskTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/BackgroundWaterCover.dds");
	IShader* TankLowerMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankLowerMaskTexture);
	TankLowerMask = Graphics->CreateBillboard(TankLowerMaskShader, 4);

	State = GameState::Setup;

	SoundManager::PlayMusic("GameMenu");
}

void Game::StartGame()
{
	curentSessionTime = 0;
	IsConnected = false;
	WaterSpeed = WaterLickSpeed;
	SetupEachRing();
	WaterTank1->FillTank();
	WaterTank1->Reset();
	WaterTank2->EmptyTank();
	WaterTank2->Reset();
	UISystem::EnableCanvasByID("GameCanvas");
	State = GameState::Playing;
}

void Game::SetupEachRing()
{
	for (size_t i = 0; i < RingHolderGrid.size(); i++)
	{
		for (size_t j = 0; j < RingHolderGrid[i].size(); j++)
		{
			RingHolderGrid[i][j]->SetupRings();
		}
	}

	SelectedRingHolder->Activate();
}

void Game::HandlePauseInput()
{
	if (Input->IsPressed(InputAction::SpecialRight))
	{
		TogglePause();
		return;
	}
}

void Game::UpdateRingSelection()
{
	if (IsConnected) return;

	if (Input->IsPressed(InputAction::TriggerLeft))
	{
		// Change ring selection towards outer
		SelectedRingHolder->UpdateRingSelection(-1);
	}
	else if (Input->IsPressed(InputAction::TriggerRight))
	{
		// Change ring selection towards inner
		SelectedRingHolder->UpdateRingSelection(1);
	}

	if (Input->IsPressed(InputAction::ButtonBottom))
	{
		ValidateAllRings();
	}

	if (Input->IsPressed(InputAction::DirectionPadTop))
	{
		SwitchToNextRingHolder(0, 1);
	}
	else if (Input->IsPressed(InputAction::DirectionPadBottom))
	{
		SwitchToNextRingHolder(0, -1);
	}
	else if (Input->IsPressed(InputAction::DirectionPadLeft))
	{
		SwitchToNextRingHolder(1, 0);
	}
	else if (Input->IsPressed(InputAction::DirectionPadRight))
	{
		SwitchToNextRingHolder (-1, 0);
	}
}

void Game::UpdateSelectedRingRotation()
{
	if (IsConnected) return;	


	if (Input->GetValue(InputAction::RightStickXAxis) != 0)
	{
		if (!hasStartedWeelSound)
		{
			hasStartedWeelSound = true;
			SoundManager::PlayAudio("RingRotation", true);
		}
	}
	else
	{
		if (hasStartedWeelSound)
		{
			hasStartedWeelSound = false;
			SoundManager::StopAudio("RingRotation");
		}
	}

	SelectedRingHolder->UpdateSelectedRingRotation(Input->GetValue(InputAction::RightStickXAxis));
}

void Game::UpdateTanks()
{
	WaterTank1->Update();
	WaterTank2->Update();
	TransferWater();
}

void Game::HandleHintInput()
{
	if (Input->IsPressed(InputAction::ButtonTop))
	{
		Text* hintText = UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("hintText");
		if (!hintText) return;
		if (ShowHint)
		{
			ShowHint = false;
			hintText->SetText(L"Hint Off");
			hintText->SetColor(Vector4(1, 0, 0, 1));

			Text* debugText = UISystem::GetCanvasUIByID(GameCanvasID)->GetUIObjectByID<Text>("DebugText");
			if (debugText)
			{
				debugText->SetText(L"");
			}
		}
		else
		{
			ShowHint = true;
			hintText->SetText(L"Hint On");
			hintText->SetColor(Vector4(0, 1, 0, 1));
		}
	}
}

void Game::HandleHint()
{
	if(!ShowHint) return;

	Text* debugText = UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText");
	if (debugText)
	{
		std::wstring message = L"Selected Ring is ";
		Vector4 color = Vector4(1, 1, 1, 1);
		int correctRings = SelectedRingHolder->GetNumOfCorrectRings();

		// set message to far, close , correct using switch
		switch (correctRings)
		{
		case 0:
			message += L"Wrong";
			color = Vector4(1, 0, 0, 1);
			break;
		case 1:
			message += L"Getting Close";
			color = Vector4(1, 1, 0, 1);
			break;
		case 2:
			message += L"Almost Correct";
			color = Vector4(0, 1, 0, 1);
			break;
		case 3:
			message += L"Correct";
			color = Vector4(0, 1, 0, 1);
		default:
			break;
		}
		debugText->SetText(message);
		debugText->SetColor(color);
	}
}

void Game::SwitchToNextRingHolder(int row, int col)
{
	if (IsConnected) return;

	SelectedRingHolder->Deactivate();

	currentRow += row;
	currentCol += col;

	if (currentRow < 0)
	{
		currentRow = RingHolderGrid.size() - 1;
	}
	else if (currentRow >= RingHolderGrid.size())
	{
		currentRow = 0;
	}

	if (currentCol < 0)
	{
		currentCol = RingHolderGrid[currentRow].size() - 1;
	}
	else if (currentCol >= RingHolderGrid[currentRow].size())
	{
		currentCol = 0;
	}

	SelectedRingHolder = RingHolderGrid[currentRow][currentCol];
	SelectedRingHolder->Activate();

	SoundManager::PlayOneShot("Switch Ring",true);
}

void Game::ValidateAllRings()
{
	bool allRingsValid = true;
	for (size_t i = 0; i < RingHolderGrid.size(); i++)
	{
		for (size_t j = 0; j < RingHolderGrid[i].size(); j++)
		{
			if (!RingHolderGrid[i][j]->ValidateRings())
			{
				allRingsValid = false;
				break;
			}
		}		
	}

	if (allRingsValid)
	{
		OnSuccess();
	}
}

void Game::OnSuccess()
{
	for (size_t i = 0; i < RingHolderGrid.size(); i++)
	{
		for (size_t j = 0; j < RingHolderGrid[i].size(); j++)
		{
			RingHolderGrid[i][j]->CorrectRings();
		}
	}
	IsConnected = true;
	WaterTank1->SetIsConnected(true);
	WaterSpeed = WaterTransferSpeed;
	SoundManager::PlayAudio ("WaterTransfer", true);
}

void Game::OnFirstTankEmpty()
{
	SoundManager::StopAudio("WaterTransfer");

	if (WaterTank2->GetNormalizedWaterLevel() >= 0.1)
	{
		State = GameState::Win;
		float normalizedWaterLevel = WaterTank2->GetNormalizedWaterLevel();
		CanvasUI* winMenuCanvas = UISystem::GetCanvasUIByID(WinMenuCanvasID);
		UISystem::EnableCanvas(winMenuCanvas);
		winMenuCanvas->GetUIObjectByID<Image>("Star1")->SetEnabled(normalizedWaterLevel >= Star1Threshold);
		winMenuCanvas->GetUIObjectByID<Image>("Star2")->SetEnabled(normalizedWaterLevel >= Star2Threshold);
		winMenuCanvas->GetUIObjectByID<Image>("Star3")->SetEnabled(normalizedWaterLevel >= Star3Threshold);

		winMenuCanvas->GetUIObjectByID<Text>("TimeText")->SetText(GetTimeString(curentSessionTime));

		int starts = normalizedWaterLevel >= Star3Threshold ? 3 : normalizedWaterLevel >= Star2Threshold ? 2 : normalizedWaterLevel >= Star1Threshold ? 1 : 0;
		PlayerDataInstance->AddSession(curentSessionTime, starts);
		winMenuCanvas->GetUIObjectByID<Text>("leaderboardText")->SetText(GetLeaderboardInfo());
		SavedPlayerData();

		SoundManager::PlayOneShot("Win");
	}
	else
	{
		State = GameState::GameOver;
		UISystem::EnableCanvasByID(GameOverCanvasID);
		UISystem::GetCanvasUIByID(GameOverCanvasID)->GetUIObjectByID<Text>("leaderboardText")->SetText(GetLeaderboardInfo());
		SoundManager::PlayOneShot("Lose");
	}
}

void Game::TransferWater()
{
	WaterTank1->UpdateWaterLevel(-1, WaterSpeed);

	if (IsConnected)
	WaterTank2->UpdateWaterLevel(1, WaterSpeed);
}

void Game::HandGameTime()
{
	if (currentSessionTimer > currentSessionTimerInterval)
	{
		curentSessionTime ++;
		UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("TimeText")->SetText(GetTimeString(curentSessionTime));
		currentSessionTimer = 0;
	}
	else
	{
		currentSessionTimer += Time::GetDeltaTime();
	}
}

void Game::TogglePause()
{
	if (State != GameState::Paused)
	{
		State = GameState::Paused;
		UISystem::EnableCanvasByID("PauseMenuCanvas");

		Text* hintText = UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("hintText");
		if (hintText)
		{
			hintText->SetText(ShowHint ? L"Hint On" : L"Hint Off");
			hintText->SetColor(ShowHint ? Vector4(0, 1, 0, 1) : Vector4(1, 0, 0, 1));
		}

		SetMusicVolume(MusicVolume);
	}
	else
	{
		State = GameState::Playing;
		UISystem::EnableCanvasByID("GameCanvas");
	}
}

void Game::HandleVolumeChange()
{
	if (Input->IsPressed(InputAction::DirectionPadLeft))
	{
		SetMusicVolume(MusicVolume - 10);
	}
	else if (Input->IsPressed(InputAction::DirectionPadRight))
	{
		SetMusicVolume(MusicVolume + 10);
	}
}

void Game::SetMusicVolume(float value)
{
	MusicVolume = CLAMP(value, 0, 100);
	SoundManager::SetGlobalVolume((float)MusicVolume / 100);

	switch (State)
	{
	case Setup:
		UISystem::GetCanvasUIByID(MainMenuCanvasID)->GetUIObjectByID<Text>("VolumeText")->SetText(L"Volume: " + std::to_wstring(MusicVolume));
		break;
	case Paused:
		UISystem::GetCanvasUIByID(PauseMenuCanvasID)->GetUIObjectByID<Text>("VolumeText")->SetText(L"Volume: " + std::to_wstring(MusicVolume));
		break;
		default: break;
	}

	PlayerDataInstance->SetMusicVolume(MusicVolume);
	SavedPlayerData();
}

void Game::LoadPlayerData()
{
	PlayerDataInstance = new PlayerData(50);

	if(!ObjectSerializer::LoadFromFile(*PlayerDataInstance, PlayerDataFileName))
	{
		ObjectSerializer::SaveToFile(*PlayerDataInstance, PlayerDataFileName);
	}

	MusicVolume = PlayerDataInstance->GetMusicVolume();
}

void Game::SavedPlayerData()
{
	ObjectSerializer::SaveToFile(*PlayerDataInstance, PlayerDataFileName);
}

std::wstring Game::GetLeaderboardInfo()
{
	std::wstring leaderboardInfo = L"";
	std::vector<SessionData*> sessions = PlayerDataInstance->GetSessions();
	PlayerDataInstance->SortSessions();
	for (size_t i = 0; i < sessions.size(); i++)
	{
		std::wstring prefix = std::to_wstring(i + 1) + L". ";
		leaderboardInfo += prefix + GetTimeString(sessions[i]->Time) + L" - " + std::to_wstring(sessions[i]->Stars) + L" Stars \n\n";
	}

	return leaderboardInfo;
}

std::wstring Game::GetTimeString(float time)
{
	int minutes = static_cast<int>(time) / 60;
	int seconds = static_cast<int>(time) % 60;
	std::wstring timeString = std::to_wstring(minutes) + L"m:" + std::to_wstring(seconds) + L"s";
	return timeString;
}
