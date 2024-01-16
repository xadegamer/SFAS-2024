#include "WaterTank.h"

#include "Game/TypeHelpers.h"
#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"
#include "Game/AnimatedSprite.h"
#include "Engine/ParticleSystem.h"

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

	ITexture* TankMaskTexture = Graphics->CreateTexture(L"Resource/Textures/BG_GraySmall.dds");
	IShader* TankMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankMaskTexture);
	TankMask = Graphics->CreateBillboard(TankMaskShader, 3);

	ITexture* ClockBGTexture = Graphics->CreateTexture(L"Resource/Textures/ClockBG.dds");
	IShader* ClockBGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ClockBGTexture);
	ClockBG = Graphics->CreateBillboard(ClockBGShader, 4);
	ClockBG->SetScale(.2, .2);

	ITexture* ClockNeedleTexture = Graphics->CreateTexture(L"Resource/Textures/ClockNeedle.dds");
	IShader* ClockNeedleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ClockNeedleTexture);
	ClockNeedle = Graphics->CreateBillboard(ClockNeedleShader, 5);
	ClockNeedle->SetScale(.2, .2);
	// 3.8 ClockNeedle->SetRotation(2.3);


	TankWaterAnimation = new AnimatedSprite(.25f, true);
	TankWaterAnimation->AddFrame(TankFill1);
	TankWaterAnimation->AddFrame(TankFill2);

	TankBody->SetScale(1.5, 2);
	TankWaterAnimation->SetScale(Vector2(1.4, 2));
	TankMask->SetScale(1.4, 2);

	FullWaterLevel = 0;
	NoWaterLevel = -200;

	waterSpeed = 5.0f;

	triggerdEvent = false;

	waterSplashInterval = 0.5f;

	ClockOffset = -100.0f;
	ClockNeedleRotationOffset = 125.0f;
}

WaterTank::~WaterTank()
{
}

void WaterTank::SetPosition(Vector2 position)
{
	Position = position;
	TankWaterAnimation->SetPosition(Position);

	TankBody->SetPosition(Position.x, Position.y);
	TankMask->SetPosition(Position.x, Position.y + NoWaterLevel);

	ClockBG->SetPosition(Position.x, Position.y + ClockOffset);
	ClockNeedle->SetPosition(Position.x, Position.y + ClockOffset);
}

void WaterTank::SetScale(Vector2 scale)
{
	Scale = scale;
	TankWaterAnimation->SetScale(scale);
	TankBody->SetScale(scale.x, scale.y);
}

void WaterTank::SetRotation(float rotation)
{
	Rotation = rotation;
	TankWaterAnimation->SetRotation(rotation);
	TankBody->SetRotation(rotation);
}

void WaterTank::SetVisible(bool visible)
{
	TankWaterAnimation->SetVisible(visible);
	TankBody->SetVisible(visible);
	TankMask->SetVisible(visible);
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
	SetClockRotation();

	if (input == -1 && !IsConnected)
	{
		SplashWater();
	}

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
	TankWaterAnimation->SetPosition(Vector2(TankWaterAnimation->GetXPosition(), level));
}

void WaterTank::SplashWater()
{
	if (TankWaterAnimation->GetYPosition() == FullWaterLevel || TankWaterAnimation->GetYPosition() == NoWaterLevel) return;

	if (waterSplashTimer > 0)
	{
		waterSplashTimer -= Time::GetDeltaTime();
	}
	else
	{
		waterSplashTimer = waterSplashInterval;
		float xPosition = GetXPosition() + 70;
		float yPosition = GetYPosition() - 70;
		ParticleSystem::Emit(Vector2(xPosition, yPosition), Vector2(0.2f, 0.2f), ParticleDirection::Cicular, 50, 100, 1.0f);
	}
}

void WaterTank::SetClockRotation()
{
	float normalised = GetNormalizedWaterLevel() * -1;
	float degrees = normalised * 270;
	ClockNeedle->SetRotation(DEG2RAD(degrees) + DEG2RAD(ClockNeedleRotationOffset));
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
