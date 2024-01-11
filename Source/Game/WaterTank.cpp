#include "WaterTank.h"

#include "Game/TypeHelpers.h"
#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"
#include "Game/AnimatedSprite.h"

#include "UI/UISystem.h"

WaterTank::WaterTank(IGraphics* Graphics)
{
	ITexture* TankBodyTexture = Graphics->CreateTexture(L"Resource/Textures/Btn_H.dds");
	IShader* TankBodyShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankBodyTexture);
	TankBody = Graphics->CreateBillboard(TankBodyShader, 1);

	ITexture* TankFill1Texture = Graphics->CreateTexture(L"Resource/Textures/Btn.dds");
	IShader* TankFill1Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankFill1Texture);
	IRenderable* TankFill1 = Graphics->CreateBillboard(TankFill1Shader, 2);

	ITexture* TankFill2Texture = Graphics->CreateTexture(L"Resource/Textures/100x Light Blue.dds");
	IShader* TankFill2Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankFill2Texture);
	IRenderable* TankFill2 = Graphics->CreateBillboard(TankFill2Shader, 2);

	TankWaterAnimation = new AnimatedSprite(.25f, true);
	TankWaterAnimation->AddFrame(TankFill1);
	TankWaterAnimation->AddFrame(TankFill2);

	ITexture* TankMaskTexture = Graphics->CreateTexture(L"Resource/Textures/BG_GraySmall.dds");
	IShader* TankMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankMaskTexture);
	TankMask = Graphics->CreateBillboard(TankMaskShader, 3);

	TankBody->SetScale(1.5, 2);
	TankWaterAnimation->SetScale(1.4, 2);
	TankMask->SetScale(1.4, 2);

	FullWaterLevel = 0;
	NoWaterLevel = -200;

	waterSpeed = 5.0f;

	triggerdEvent = false;
}

WaterTank::~WaterTank()
{
}

void WaterTank::SetPosition(float x, float y)
{
	TankWaterAnimation->SetPosition(x, y);
	TankBody->SetPosition(x, y);
	TankMask->SetPosition(x, y + NoWaterLevel);
}

void WaterTank::SetScale(float x, float y)
{
	TankWaterAnimation->SetScale(x, y);
	TankBody->SetScale(x, y);
}

void WaterTank::Update()
{
	TankWaterAnimation->Update();
}

void WaterTank::UpdateWaterLevel(float input)
{
	float currentWaterLevel = TankWaterAnimation->GetYPosition();
	float newWaterLevel = currentWaterLevel + input * Time::GetDeltaTime() * waterSpeed;

	newWaterLevel = CLAMP(newWaterLevel, NoWaterLevel, FullWaterLevel);

	SetWaterLevel (newWaterLevel);

	ValidateWaterLevel();

	//UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText")->SetText(std::to_wstring(TankFill->GetTransform().PositionY));
}

void WaterTank::ValidateWaterLevel()
{
	if (TankWaterAnimation->GetYPosition() == FullWaterLevel && !triggerdEvent)
	{
		if(OnFullEvent) OnFullEvent();
		triggerdEvent = true;
	}
	else if (TankWaterAnimation->GetYPosition() == NoWaterLevel && !triggerdEvent)
	{
		if (OnEmptyEvent) OnEmptyEvent();
		triggerdEvent = true;
	}
}

void WaterTank::SetWaterLevel(float level)
{
	TankWaterAnimation->SetPosition(TankWaterAnimation->GetXPosition(), level);
}

float WaterTank::GetNormalizedWaterLevel()
{
	float currentWaterLevel = TankWaterAnimation->GetYPosition();
	float normalizedWaterLevel = (currentWaterLevel - NoWaterLevel) / (FullWaterLevel - NoWaterLevel);
	return normalizedWaterLevel;
}

void WaterTank::Reset()
{
	triggerdEvent = false;
}	