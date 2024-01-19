#include "WaterTank.h"

#include "Game/TypeHelpers.h"
#include "Engine/IRenderable.h"
#include "Engine/IGraphics.h"
#include "Engine/Time.h"
#include "Game/AnimatedSprite.h"
#include "Engine/ParticleSystem.h"

#include "UI/UISystem.h"

WaterTank::WaterTank(IGraphics* Graphics, bool start)
{
	WaterOffset = start ? -70.0f : 90.0f;

	std::wstring TankBodyPath = L"Resource/Textures/WaterTank/" + std::wstring(start ? L"StartWaterTank" : L"EndWaterTank") + L".dds";
	ITexture* TankBodyTexture = Graphics->CreateTexture(TankBodyPath.c_str());
	IShader* TankBodyShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankBodyTexture);
	TankBody = Graphics->CreateBillboard(TankBodyShader, 1);

	ITexture* TankWaterBgTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterBG.dds");
	IShader* TankWaterBgShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterBgTexture);
	TankWaterBg = Graphics->CreateBillboard(TankWaterBgShader, 2);

	ITexture* TankWaterAnim1Texture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterAnim1.dds");
	IShader* TankWater1Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterAnim1Texture);
	IRenderable* TankWater1 = Graphics->CreateBillboard(TankWater1Shader, 3);

	ITexture* TankWaterAnim2Texture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterAnim2.dds");
	IShader* TankWater2Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterAnim2Texture);
	IRenderable* TankWater2 = Graphics->CreateBillboard(TankWater2Shader, 3);

	ITexture* TankWaterAnim3Texture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterAnim3.dds");
	IShader* TankWater3Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterAnim3Texture);
	IRenderable* TankWater3 = Graphics->CreateBillboard(TankWater3Shader, 3);

	ITexture* TankWaterAnim4Texture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterAnim4.dds");
	IShader* TankWater4Shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterAnim4Texture);
	IRenderable* TankWater4 = Graphics->CreateBillboard(TankWater4Shader, 3);

	TankWaterAnimation = new AnimatedSprite(.1f, true);
	TankWaterAnimation->AddFrame(TankWater1);
	TankWaterAnimation->AddFrame(TankWater2);
	TankWaterAnimation->AddFrame(TankWater3);
	TankWaterAnimation->AddFrame(TankWater4);

	ITexture* WaterLevelMarkerTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterLevelMarker.dds");
	IShader* WaterLevelMarkerShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", WaterLevelMarkerTexture);
	WaterLevelMarker = Graphics->CreateBillboard(WaterLevelMarkerShader, 6);
	WaterLevelMarker->SetVisible(!start);

	std::wstring BodyMaskPath = L"Resource/Textures/WaterTank/" + std::wstring(start ? L"StartTankBodyMask" : L"EndTankBodyMask") + L".dds";
	ITexture* TankBodyMaskTexture = Graphics->CreateTexture(BodyMaskPath.c_str());
	IShader* TankBodyMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankBodyMaskTexture);
	TankBodyMask = Graphics->CreateBillboard(TankBodyMaskShader, 4);

	ITexture* TankWaterOverlayTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/WaterOverlay.dds");
	IShader* TankWaterOverlayShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankWaterOverlayTexture);
	TankWaterOverlay = Graphics->CreateBillboard(TankWaterOverlayShader, 5);

	ITexture* TankLowerMaskTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/BackgroundWaterCover.dds");
	IShader* TankLowerMaskShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", TankLowerMaskTexture);
	TankLowerMask = Graphics->CreateBillboard(TankLowerMaskShader, 4);

	FullWaterLevel = 15;
	NoWaterLevel = -160;

	triggerdEvent = false;

	waterSplashInterval = 0.5f;

	ClockOffset = -180.0f;
	ClockNeedleRotationOffset = 125.0f;

	ITexture* ClockBGTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/ClockBG.dds");
	IShader* ClockBGShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ClockBGTexture);
	ClockBG = Graphics->CreateBillboard(ClockBGShader, 5);
	ClockBG->SetScale(.2, .2);

	ITexture* ClockNeedleTexture = Graphics->CreateTexture(L"Resource/Textures/WaterTank/ClockNeedle.dds");
	IShader* ClockNeedleShader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", ClockNeedleTexture);
	ClockNeedle = Graphics->CreateBillboard(ClockNeedleShader, 6);
	ClockNeedle->SetScale(.2, .2);
}

WaterTank::~WaterTank()
{
}

void WaterTank::SetPosition(Vector2 position)
{
	Position = position;

	TankBody->SetPosition(Position.x, Position.y);
	TankBodyMask->SetPosition(Position.x, Position.y);

	TankWaterBg->SetPosition(Position.x + WaterOffset, Position.y);
	TankWaterAnimation->SetPosition(Position + Vector2(WaterOffset, 0));
	TankWaterOverlay->SetPosition(Position.x + WaterOffset, Position.y);

	//TankLowerMask->SetPosition(Position.x + WaterOffset, Position.y + NoWaterLevel);
	//TankLowerMask->SetPosition(Position.x + 500, Position.y + NoWaterLevel);

	WaterLevelMarker->SetPosition(Position.x + WaterOffset, GetWaterPosition(1) + 80);

	ClockBG->SetPosition(Position.x + WaterOffset, Position.y + ClockOffset);
	ClockNeedle->SetPosition(Position.x + WaterOffset, Position.y + ClockOffset);
}

void WaterTank::SetScale(Vector2 scale)
{
	Scale = scale;
	TankWaterAnimation->SetScale(scale);
	TankBody->SetScale(scale.x, scale.y);
	TankLowerMask->SetScale(scale.x, scale.y);
	TankBodyMask->SetScale(scale.x, scale.y);
	TankWaterBg->SetScale(scale.x, scale.y);
	TankWaterOverlay->SetScale(scale.x, scale.y);
	ClockBG->SetScale(scale.x, scale.y);
	ClockNeedle->SetScale(scale.x, scale.y);
}

void WaterTank::SetRotation(float rotation)
{
	Rotation = rotation;
	TankWaterAnimation->SetRotation(rotation);
	TankBody->SetRotation(rotation);
	TankLowerMask->SetRotation(rotation);
}

void WaterTank::SetVisible(bool visible)
{
	TankWaterAnimation->SetVisible(visible);
	TankBody->SetVisible(visible);
	TankLowerMask->SetVisible(visible);
	TankBodyMask->SetVisible(visible);
	TankWaterBg->SetVisible(visible);
	TankWaterOverlay->SetVisible(visible);
	ClockBG->SetVisible(visible);
	ClockNeedle->SetVisible(visible);
}

void WaterTank::Update()
{
	TankWaterAnimation->Update();
}

void WaterTank::UpdateWaterLevel(float input, float speed)
{
	float currentWaterLevel = TankWaterAnimation->GetYPosition();
	float newWaterLevel = currentWaterLevel + input * Time::GetDeltaTime() * speed;

	newWaterLevel = CLAMP(newWaterLevel, NoWaterLevel, FullWaterLevel);

	SetWaterLevel (newWaterLevel);
	SetClockRotation();

	if (input == -1 && !IsConnected)
	{
		SplashWater();
	}

	ValidateWaterLevel();

	//UISystem::GetActiveCanvas()->GetUIObjectByID<Text>("DebugText")->SetText(std::to_wstring(TankWaterAnimation->GetYPosition()));
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
		float xPosition = GetXPosition() + 200;
		float yPosition = GetYPosition() - 165;
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

float WaterTank::GetWaterPosition(float normalizedWaterLevel)
{
	float waterPosition = normalizedWaterLevel * (FullWaterLevel - NoWaterLevel) + NoWaterLevel;
	return waterPosition;
}

void WaterTank::FillTank()
{
	SetWaterLevel(FullWaterLevel);
	SetClockRotation();
}

void WaterTank::EmptyTank()
{
	SetWaterLevel(NoWaterLevel);
	SetClockRotation();
}

void WaterTank::Reset()
{
	triggerdEvent = false;
}
