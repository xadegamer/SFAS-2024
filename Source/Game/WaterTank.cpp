#include "WaterTank.h"

#include "Game/TypeHelpers.h"
#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"

#include "UI/UISystem.h"

WaterTank::WaterTank(IGraphics* Graphics)
{
	ITexture* TankBodyTexture = Graphics->CreateTexture(L"Resource/Textures/Btn_H.dds");
	IShader* TankBodyShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankBodyTexture);
	TankBody = Graphics->CreateBillboard(TankBodyShader, 1);

	ITexture* TankFillTexture = Graphics->CreateTexture(L"Resource/Textures/Btn.dds");
	IShader* TankFillShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankFillTexture);
	TankFill = Graphics->CreateBillboard(TankFillShader, 2);

	ITexture* TankMaskTexture = Graphics->CreateTexture(L"Resource/Textures/BG_GraySmall.dds");
	IShader* TankMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankMaskTexture);
	TankMask = Graphics->CreateBillboard(TankMaskShader, 3);

	TankBody->SetScale(1.5, 2);
	TankFill->SetScale(1.4, 2);
	TankMask->SetScale(1.4, 2);

	FullWaterLevel = -200;
	NoWaterLevel = 0;

	waterSpeed = 15.0f;

	triggerdEvent = false;
}

WaterTank::~WaterTank()
{
}

void WaterTank::SetPosition(float x, float y)
{
	TankFill->SetPosition(x, y);
	TankBody->SetPosition(x, y);
	TankMask->SetPosition(x, y - 200);
}

void WaterTank::SetScale(float x, float y)
{
	TankFill->SetScale(x, y);
	TankBody->SetScale(x, y);
}

void WaterTank::UpdateWaterLevel(float input)
{
	float currentWaterLevel = TankFill->GetTransform().PositionY;
	float newWaterLevel = currentWaterLevel + input * Time::GetDeltaTime() * waterSpeed;
	newWaterLevel = CLAMP(newWaterLevel, FullWaterLevel,NoWaterLevel);

	SetWaterLevel (newWaterLevel);

	ValidateWaterLevel();

	//UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText")->SetText(std::to_wstring(TankFill->GetTransform().PositionY));
}

void WaterTank::ValidateWaterLevel()
{
	if (TankFill->GetTransform().PositionY == FullWaterLevel && !triggerdEvent)
	{
		if(OnFullEvent) OnFullEvent();
		triggerdEvent = true;
	}
	else if (TankFill->GetTransform().PositionY == NoWaterLevel && !triggerdEvent)
	{
		if (OnEmptyEvent) OnEmptyEvent();
		triggerdEvent = true;
	}
}

void WaterTank::SetWaterLevel(float level)
{
	TankFill->SetPosition(TankFill->GetTransform().PositionX, level);
}
