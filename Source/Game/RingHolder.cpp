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

RingHolder::RingHolder(IGraphics* Graphics, std::wstring  ringName) : Rings(), Arrow(nullptr), SelectedRing()
{
	ITexture* CentreTexture = Graphics->CreateTexture(L"Resource/Textures/Centre_Black.dds");

	std::wstring InnerTRingPath = L"Resource/Textures/" + ringName + L"/1.dds";
	std::wstring MiddleTRingPath = L"Resource/Textures/" + ringName + L"/2.dds";
	std::wstring OuterTRingPath = L"Resource/Textures/" + ringName + L"/3.dds";

	ITexture* InnerTexture = Graphics->CreateTexture (InnerTRingPath.c_str());
	ITexture* MiddleTexture = Graphics->CreateTexture (MiddleTRingPath.c_str());
	ITexture* OuterTexture = Graphics->CreateTexture( OuterTRingPath.c_str());

	ITexture* ArrowTexture = Graphics->CreateTexture(L"Resource/Textures/Arrow.dds");

	IShader* CentreShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", CentreTexture);
	IShader* InnerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerTexture);
	IShader* MiddleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleTexture);
	IShader* OuterShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterTexture);
	IShader* ArrowShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ArrowTexture);

	Centre = Graphics->CreateBillboard(CentreShader);

	Rings[static_cast<unsigned int>(RingLayer::Inner)] = Graphics->CreateBillboard(InnerShader);
	Rings[static_cast<unsigned int>(RingLayer::Middle)] = Graphics->CreateBillboard(MiddleShader);
	Rings[static_cast<unsigned int>(RingLayer::Outer)] = Graphics->CreateBillboard(OuterShader);
	Arrow = Graphics->CreateBillboard(ArrowShader,2);

	std::srand(static_cast<unsigned int>(std::time(0)));

	SelectedRing = RingLayer::Outer;

	Centre->SetVisible(false);
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
	/*	Rings[Ring]->SetRotation(static_cast<float>(fmod(rand(), Pie)));*/
		Rings[Ring]->SetRotation(0.0f);
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
		if (OnSuccessEvent) OnSuccessEvent();
	}
	else
	{
		//SetupRings();
	}
}

void RingHolder::SetPosition(float x, float y)
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetPosition(x, y);
	}

	Arrow->SetPosition(x, y);
	Centre->SetPosition(x, y);
}

void RingHolder::SetScale(float x, float y)
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetScale(x, y);
	}

	Arrow->SetScale(x, y);
	Centre->SetScale(x, y);
}

void RingHolder::Activate()
{
	Centre->SetVisible(true);
}

void RingHolder::Deactivate()
{
	Centre->SetVisible(false);
}

float RingHolder::GetSelectedRingRotation()
{
	return Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation;
}
