#include "RingHolder.h"

#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"

#include <ctime>
#include <math.h>

constexpr float Pie = 3.14159265359f;
constexpr float TwoPies = Pie * 2.0f;
constexpr float SpinSpeed = 5.0f;
constexpr float WinTolerance = 0.5;

RingHolder::RingHolder(IGraphics* Graphics, std::wstring  ringName) : Rings(), SelectedRing()
{
	ITexture* CentreTexture = Graphics->CreateTexture(L"Resource/Textures/Centre_Black.dds");

	std::wstring InnerTRingPath = L"Resource/Textures/" + ringName + L"/1.dds";
	std::wstring MiddleTRingPath = L"Resource/Textures/" + ringName + L"/2.dds";
	std::wstring OuterTRingPath = L"Resource/Textures/" + ringName + L"/3.dds";

	ITexture* InnerTexture = Graphics->CreateTexture (InnerTRingPath.c_str());
	ITexture* MiddleTexture = Graphics->CreateTexture (MiddleTRingPath.c_str());
	ITexture* OuterTexture = Graphics->CreateTexture( OuterTRingPath.c_str());

	IShader* CentreShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", CentreTexture);
	IShader* InnerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerTexture);
	IShader* MiddleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleTexture);
	IShader* OuterShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterTexture);

	ITexture* InnerHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/1H.dds");
	ITexture* MiddleHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/2H.dds");
	ITexture* OuterHighlightTexture = Graphics->CreateTexture(L"Resource/Textures/3H.dds");

	IShader* InnerHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", InnerHighlightTexture);
	IShader* MiddleHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", MiddleHighlightTexture);
	IShader* OuterHighlightShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", OuterHighlightTexture);


	Centre = Graphics->CreateBillboard(CentreShader);

	Rings[static_cast<unsigned int>(RingLayer::Inner)] = Graphics->CreateBillboard(InnerShader);
	Rings[static_cast<unsigned int>(RingLayer::Middle)] = Graphics->CreateBillboard(MiddleShader);
	Rings[static_cast<unsigned int>(RingLayer::Outer)] = Graphics->CreateBillboard(OuterShader);

	RingsHighlight[static_cast<unsigned int>(RingLayer::Inner)] = Graphics->CreateBillboard(InnerHighlightShader,2);
	RingsHighlight[static_cast<unsigned int>(RingLayer::Middle)] = Graphics->CreateBillboard(MiddleHighlightShader,2);
	RingsHighlight[static_cast<unsigned int>(RingLayer::Outer)] = Graphics->CreateBillboard(OuterHighlightShader,2);

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
		//Rings[Ring]->SetRotation(static_cast<float>(fmod(rand(), Pie)));
		RingsHighlight [Ring]->SetVisible(Ring == SelectedRing);
	}
}

void RingHolder::UpdateRingSelection(int dir)
{
	int selectionChange = dir;
	RingsHighlight[static_cast<int>(SelectedRing)]->SetVisible(false);
	SelectedRing = static_cast<RingLayer>(CLAMP(static_cast<int>(SelectedRing) + selectionChange, 0, NumberOfRings - 1));
	RingsHighlight[static_cast<int>(SelectedRing)]->SetVisible(true);
}

void RingHolder::UpdateSelectedRingRotation(float input)
{
	float delta = input * SpinSpeed * Time::GetDeltaTime();
	float rotation = Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation;
	float newRotation = static_cast<float>(fmod(rotation + delta, TwoPies));
	Rings[static_cast<int>(SelectedRing)]->SetRotation(newRotation);
}

float RingHolder::CheckForSuccess()
{
	float totalRotationDifference = 0.0f;

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		totalRotationDifference += abs(Rings[Ring]->GetTransform().Rotation);
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

	return averageRotationDifference;
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
	float totalRotationDifference = 0.0f;

	//for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	//{
	//	totalRotationDifference += abs(Rings[Ring]->GetTransform().Rotation);
	//}

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		totalRotationDifference += Rings[Ring]->GetTransform().Rotation;
	}

	float averageRotationDifference = totalRotationDifference / NumberOfRings;

	return averageRotationDifference;

	//return abs(Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation);
	//return ConvertRotation (abs(Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation));

	//return  Rings[static_cast<int>(SelectedRing)]->GetTransform().Rotation + TwoPies;
}

float RingHolder::ConvertRotation(float rotation)
{   
	return  static_cast<float>(fmod (rotation , TwoPies));

	//float degrees = rotation * 60.0f;
	//if (degrees > 180.0)
	//	degrees -= 360.0;
	//return degrees;
}

void RingHolder::SetPosition(Vector2 position)
{
	Position = position;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetPosition(Position.x, Position.y);
	}

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		RingsHighlight[Ring]->SetPosition(Position.x, Position.y);
	}

	Centre->SetPosition(Position.x, Position.y);
}

void RingHolder::SetScale(Vector2 scale)
{
	Scale = scale;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetScale(Scale.x, Scale.y);
	}

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		RingsHighlight[Ring]->SetScale(Scale.x, Scale.y);
	}

	Centre->SetScale(Scale.x, Scale.y);
}

void RingHolder::SetRotation(float rotation)
{
	Rotation = rotation;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetRotation(Rotation);
	}

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		RingsHighlight[Ring]->SetRotation(Rotation);
	}

	Centre->SetRotation(Rotation);
}

void RingHolder::SetVisible(bool visible)
{
	Visible = visible;
	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		Rings[Ring]->SetVisible(Visible);
	}

	for (unsigned int Ring = 0; Ring < NumberOfRings; ++Ring)
	{
		RingsHighlight[Ring]->SetVisible(Visible);
	}

	Centre->SetVisible(Visible);
}

float RingHolder::GetXPosition()
{
	return Position.x;
}

float RingHolder::GetYPosition()
{
	return Position.y;
}

float RingHolder::GetRotation()
{
	return Rotation;
}
