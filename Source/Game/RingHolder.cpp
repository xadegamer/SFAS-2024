#include "RingHolder.h"

#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"

#include <ctime>
#include <math.h>

#define CLAMP(v, x, y) fmin(fmax(v, x), y)
constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float DeltaTime = 0.016f;
constexpr float SpinSpeed = 0.1f;
constexpr float WinTolerance = Pie / 90.0f;

RingHolder::RingHolder(IGraphics* Graphics) : Rings(), Arrow(nullptr), SelectedRing()
{
	ITexture* InnerTexture = Graphics->CreateTexture(L"Resource/Textures/InnerRing.dds");
	ITexture* MiddleTexture = Graphics->CreateTexture(L"Resource/Textures/MiddleRing.dds");
	ITexture* OuterTexture = Graphics->CreateTexture(L"Resource/Textures/OuterRing.dds");
	ITexture* ArrowTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow.dds");

	IShader* InnerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerTexture);
	IShader* MiddleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleTexture);
	IShader* OuterShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterTexture);
	IShader* ArrowShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowTexture);

	Rings[static_cast<unsigned int>(RingLayer::Inner)] = Graphics->CreateBillboard(InnerShader);
	Rings[static_cast<unsigned int>(RingLayer::Middle)] = Graphics->CreateBillboard(MiddleShader);
	Rings[static_cast<unsigned int>(RingLayer::Outer)] = Graphics->CreateBillboard(OuterShader);
	Arrow = Graphics->CreateBillboard(ArrowShader);

	std::srand(static_cast<unsigned int>(std::time(0)));

	SelectedRing = RingLayer::Outer;
}

RingHolder::~RingHolder()
{

}

void RingHolder::AddSuccessEventListener(std::function<void()> onSuccessEvent)
{
	OnSuccessEvent = onSuccessEvent;
}

void RingHolder::SetupRings()
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetRotation(static_cast<float>(fmod(rand(), Pie)));
	}

	Arrow->SetRotation(static_cast<float>(fmod(rand(), Pie)));
}

void RingHolder::UpdateRingSelection(int dir)
{
	int selectionChange = dir;
	SelectedRing = static_cast<RingLayer>(CLAMP(static_cast<int>(SelectedRing) + selectionChange, 0, NumberOfRings - 1));
}

void RingHolder::UpdateSelectedRingRotation(float input)
{
	float delta = input * SpinSpeed * DeltaTime;
	float rotation = Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation;
	float newRotation = static_cast<float>(fmod(rotation + delta, TwoPies));
	Rings[static_cast<int>(SelectedRing)]->SetRotation(newRotation);
}

void RingHolder::CheckForSuccess()
{
	float totalRotationDifference = 0.0f;
	float arrowRotation = Arrow->GetTransform().Rotation + TwoPies;

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		totalRotationDifference += abs(arrowRotation - (Rings[Ring]->GetTransform().Rotation + TwoPies));
	}

	float averageRotationDifference = totalRotationDifference / NumberOfRings;

	if (averageRotationDifference < WinTolerance)
	{
		// Win
		if (OnSuccessEvent) OnSuccessEvent();

	}
	else
	{
		// Lose
		totalRotationDifference = 0.0f;
	}
}

void RingHolder::SetPosition(float x, float y)
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetPosition(x, y);
	}

	Arrow->SetPosition(x, y);
}

void RingHolder::SetScale(float x, float y)
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetScale(x, y);
	}

	Arrow->SetScale(x, y);
}
