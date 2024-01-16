#pragma once
#include "SimpleMath.h"
#include <DirectXColors.h>
#include <string>
#include <iostream>

typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::SimpleMath::Vector2 Vector2;
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector4 Vector4;

#define CLAMP(v, x, y) fmin(fmax(v, x), y)