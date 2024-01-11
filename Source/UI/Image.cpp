#include "Image.h"
#include "Engine/IGraphics.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IRenderable.h"

Image::Image(std::string Id, IGraphics* Graphics, std::wstring texturePath, Vector2 pos, int layer, Vector2 scale)
{
	this->Id = Id;
	this->Position = pos;
	this->Scale = scale;
	this->Layer = layer;

	ITexture* texture = Graphics->CreateTexture(texturePath.c_str());
	IShader* shader = Graphics->CreateShader(L"Resource/Shaders/UnlitColor.fx", "VS_Main", "vs_4_0", "PS_Main", "ps_4_0", texture);
	sprite = Graphics->CreateBillboard(shader, Layer);
	sprite->SetPosition(pos.x, pos.y);
	sprite->SetScale(scale.x, scale.y);
	Disable();
}

void Image::Enable()
{
	sprite->SetVisible(true);
}

void Image::Disable()
{
	sprite->SetVisible(false);
}

void Image::Update(float deltaTime)
{
}
