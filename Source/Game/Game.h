
#pragma once

#include <vector>
#include "Engine/IApplication.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;
class RingHolder;
class WaterTank;

enum GameState { Setup, Playing, GameOver, Win};

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

	GameState State;
	std::vector<RingHolder*> RingHolders;
	int CurrentRingHolderIndex;

	IRenderable* CentrebGG;

	WaterTank* WaterTank1;
	WaterTank* WaterTank2;

	bool IsPaused;
	bool IsConnected;

	void SetUpGame();
	void StartGame();

	void SetupEachRing();

	void UpdateRingSelection();
	void UpdateSelectedRingRotation();

	void SwitchToNextRingHolder(int direction);

	void OnSuccess();
	void OnFirstTankEmpty();

	void TransferWater();

	void SetIsPaused(bool isPaused);
};

