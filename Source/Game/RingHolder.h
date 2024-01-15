#pragma once

#include <functional>
#include <string>

#include "IGameObject.h"

class IGraphics;
class ITexture;
class IShader;
class IRenderable;
class Ring;

enum RingLayer { Outer, Middle, Inner, NumRings };
static const unsigned int NumberOfRings = static_cast<int>(NumRings);

class RingHolder : public IGameObject
{

private:

	IRenderable* Centre;
	Ring* Rings[NumberOfRings];
	RingLayer SelectedRing;

	float Pie;
	float TwoPies;
	float SpinSpeed;
	float WinTolerance;

	std::function <void()> OnSuccessEvent;

public:

	RingHolder(IGraphics* Graphics, std::wstring ringName);
	virtual ~RingHolder();

	void AddSuccessEventListener(std::function <void()> onSuccessEvent);
	void SetupRings();
	void UpdateRingSelection(int input);
	void UpdateSelectedRingRotation(float input);
	float CheckForSuccess();
	void Activate();
	void Deactivate();
	float GetSelectedRingRotation();
	float ConvertRotation(float rotation);

	virtual void SetPosition(Vector2 position) override;
	virtual void SetScale(Vector2 scale) override;
	virtual void SetRotation(float rotation) override;
	virtual void SetVisible(bool visible) override;

	virtual float GetXPosition() override;
	virtual float GetYPosition() override;
	virtual float GetRotation() override;


};

