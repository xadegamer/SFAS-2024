
#pragma once

#include <vector>
#include "Engine/IApplication.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;
class RingHolder;

//enum RingLayer { Outer, Middle, Inner, NumRings };
//static const unsigned int NumberOfRings = static_cast<int>(NumRings);

enum GameState { Setup, Playing, Test, NumModes };

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

	void SetupEachRing();
	void UpdateRingSelection();
	void UpdateSelectedRingRotation();
	void UpdateRingTestSelection();
	void TestRingSolution();

	void SwitchToNextRingHolder(int direction);

	void OnSuccess();

	//IRenderable* Rings[NumberOfRings];
	//IRenderable* Arrow;
	//RingLayer SelectedRing;
	GameState State;

	std::vector<RingHolder*> RingHolders;
	int CurrentRingHolderIndex;
};

