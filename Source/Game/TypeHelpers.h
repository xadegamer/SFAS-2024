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
#define PI 3.14159265358979323846f
#define DEG2RAD(x) (x * PI / 180.0f)
#define RAD2DEG(x) (x * 180.0f / PI)