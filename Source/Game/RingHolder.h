#pragma once

#include <functional>
#include <string>

class IGraphics;
class ITexture;
class IShader;
class IRenderable;

enum RingLayer { Outer, Middle, Inner, NumRings };
static const unsigned int NumberOfRings = static_cast<int>(NumRings);

class RingHolder
{

private:

	IRenderable* Centre;
	IRenderable* Rings[NumberOfRings];
	IRenderable* RingsHighlight[NumberOfRings];
	RingLayer SelectedRing;

	std::function <void()> OnSuccessEvent;

public:

	RingHolder(IGraphics* Graphics, std::wstring ringName);
	virtual ~RingHolder();

	void AddSuccessEventListener(std::function <void()> onSuccessEvent);
	void SetupRings();
	void UpdateRingSelection(int input);
	void UpdateSelectedRingRotation(float input);
	float CheckForSuccess();

	void SetPosition(float x, float y);
	void SetScale(float x, float y);

	void Activate();
	void Deactivate();

	float GetSelectedRingRotation();

	float ConvertRotation(float rotation);
};

