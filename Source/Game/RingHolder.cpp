#include "RingHolder.h"

#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"
#include "Ring.h"

#include <ctime>
#include <math.h>

RingHolder::RingHolder(IGraphics* Graphics, std::wstring  ringName) : Rings(), SelectedRing()
{
	TwoPies = PI * 2.0f;
	SpinSpeed = 2.5f;
	WinTolerance = 0.1;

	std::wstring InnerTRingPath = L"Resource/Textures/" + ringName + L"/1.dds";
	std::wstring MiddleTRingPath = L"Resource/Textures/" + ringName + L"/2.dds";
	std::wstring OuterTRingPath = L"Resource/Textures/" + ringName + L"/3.dds";

	ITexture* InnerTexture = Graphics->CreateTexture (InnerTRingPath.c_str());
	ITexture* MiddleTexture = Graphics->CreateTexture (MiddleTRingPath.c_str());
	ITexture* OuterTexture = Graphics->CreateTexture( OuterTRingPath.c_str());

	IShader* InnerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerTexture);
	IShader* MiddleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleTexture);
	IShader* OuterShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterTexture);

	ITexture* InnerHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/1H.dds");
	ITexture* MiddleHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/2H.dds");
	ITexture* OuterHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/3H.dds");

	IShader* InnerHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerHighlightTexture);
	IShader* MiddleHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleHighlightTexture);
	IShader* OuterHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterHighlightTexture);

	for (unsigned int RingIndex = 0; RingIndex < NumberOfRings; ++RingIndex)
	{
		std::wstring mainPath = L"Resource/Textures/" + ringName  + L"/" + std::to_wstring(RingIndex + 1) + L".dds";
		ITexture* mainTexture = Graphics->CreateTexture(mainPath.c_str());
		IShader* mainShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", mainTexture);

		std::wstring highlightPath = L"Resource/Textures/" + std::to_wstring(RingIndex + 1) + L"H.dds";
		ITexture* highlightTexture = Graphics->CreateTexture(highlightPath.c_str());
		IShader* highlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", highlightTexture);


		Rings[RingIndex] = new Ring(Graphics->CreateBillboard(mainShader),Graphics->CreateBillboard(highlightShader, 2));
	}

	std::srand(static_cast<unsigned int>(std::time(0)));

	SelectedRing = RingLayer::Outer;
}

RingHolder::~RingHolder()
{
	for (unsigned int ringIndex = 0; ringIndex < NumberOfRings; ++ringIndex)
	{
		delete Rings[ringIndex];
		Rings[ringIndex] = nullptr;
	}
}

void RingHolder::AddSuccessEventListener(std::function<void()> onSuccessEvent)
{
	OnSuccessEvent = onSuccessEvent;
}

void RingHolder::SetupRings()
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		//Rings[Ring]->SetRotation(static_cast<float>(fmod(rand(), PI)));
		Rings[Ring]->ToggleHighlight(false);
	}
}

void RingHolder::UpdateRingSelection(int dir)
{
	int selectionChange = dir;
	Rings[static_cast<int>(SelectedRing)]->ToggleHighlight(false);
	SelectedRing = static_cast<RingLayer>(CLAMP(static_cast<int>(SelectedRing) + selectionChange, 0, NumberOfRings - 1));
	Rings[static_cast<int>(SelectedRing)]->ToggleHighlight(true);
}

void RingHolder::UpdateSelectedRingRotation(float input)
{
	float delta = input * SpinSpeed * Time::GetDeltaTime();
	float rotation = Rings[static_cast<int>(SelectedRing)]->GetRotation();
	float newRotation = static_cast<float>(fmod(rotation + delta, TwoPies));
	Rings[static_cast<int>(SelectedRing)]->SetRotation(newRotation);
}

bool RingHolder::ValidateRings()
{
	for (unsigned int ringIndex = 0; ringIndex < NumberOfRings; ++ringIndex)
	{
		float rotation = abs(Rings[ringIndex]->GetRotation());
		if (rotation > WinTolerance)
		{
			return false;
		}
	}
	return true;
}

void RingHolder::CheckForSuccess()
{
	if (ValidateRings())
	{
		if (OnSuccessEvent) OnSuccessEvent();
	}
	else
	{
		for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
		{
			Rings[Ring]->SetRotation(0);
			Rings[Ring]->ToggleHighlight(false);
		}
	}
}

void RingHolder::Activate()
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->ToggleHighlight(Ring == SelectedRing);
	}
}

void RingHolder::Deactivate()
{
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->ToggleHighlight(false);
	}
}

float RingHolder::GetSelectedRingRotation()
{
	float totalRotationDifference = 0.0f;

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		totalRotationDifference += abs(Rings[Ring]->GetRotation());
	}

	float averageRotationDifference = totalRotationDifference / NumberOfRings;

	return abs(Rings[static_cast<int>(SelectedRing)]->GetRotation());
}


void RingHolder::SetPosition(Vector2 position)
{
	Position = position;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetPosition(position);
	}
}

void RingHolder::SetScale(Vector2 scale)
{
	Scale = scale;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetScale(scale);
	}
}

void RingHolder::SetRotation(float rotation)
{
	Rotation = rotation;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetRotation(Rotation);
	}
}

void RingHolder::SetVisible(bool visible)
{
	Visible = visible;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetVisible(Visible);
	}
}