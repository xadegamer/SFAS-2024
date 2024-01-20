
#pragma once

#include <vector>
#include <string>
#include "Engine/IApplication.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;
class RingHolder;
class WaterTank;
class PlayerData;

enum GameState { Setup, Playing, Paused, GameOver, Win};

class Game : public IApplication
{
public:

	Game(IGraphics* Graphics, IInput* InputIn);
	virtual ~Game();

	virtual bool IsValid();
	virtual bool Load();
	virtual void Update();
	virtual void Cleanup();

private:

	int currentRow = 0; 
	int currentCol = 0;

	std::vector<std::vector<RingHolder*>> RingHolderGrid;
	RingHolder* SelectedRingHolder;

	GameState State;
	IRenderable* CentrebGG;
	WaterTank* WaterTank1;
	WaterTank* WaterTank2;
	IRenderable* TankLowerMask;

	PlayerData* PlayerDataInstance;	

	bool IsConnected;
	int MusicVolume;

	float WaterTransferSpeed;
	float WaterLickSpeed;
	float WaterSpeed;	

	float Star1Threshold;
	float Star2Threshold;
	float Star3Threshold;

	float curentSessionTime;
	float currentSessionTimer;
	float currentSessionTimerInterval;

	void SetUpGame();
	void StartGame();

	void SetupEachRing();

	void HandlePauseInput();
	void UpdateRingSelection();
	void UpdateSelectedRingRotation();
	void UpdateTanks();

	void SwitchToNextRingHolder(int row, int col);
	void ValidateAllRings();
	void OnSuccess();

	void OnFirstTankEmpty();
	void TransferWater();

	void HandGameTime();

	void TogglePause();
	void HandleVolumeChange();
	void SetMusicVolume(float value);

	void LoadPlayerData();
	void SavedPlayerData();

	std::wstring GetTimeString(float time);
};

