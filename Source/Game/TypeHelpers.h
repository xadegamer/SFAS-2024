#pragma once
#include "SimpleMath.h"
#include <DirectXColors.h>
#include <string>
#include <iostream>

typedef DirectX::SimpleMath::Matrix Matrix;
typedef DirectX::SimpleMath::Vector2 Vector2;
typedef DirectX::SimpleMath::Vector3 Vector3;
typedef DirectX::SimpleMath::Vector4 Vector4;
typedef DirectX::SimpleMath::Quaternion Quaternion;


typedef std::string Name;

#define CLAMP(v, x, y) fmin(fmax(v, x), y)

//float ConvertRotation(float rotation) {
//    // Validate the input value
//    if (rotation < 0 || rotation > 6) {
//        throw std::invalid_argument("Rotation value must be in the range 0-6.");
//    }
//
//    // Convert and return the angle in degrees
//    return static_cast<float>(rotation) * 60.0f;
//}