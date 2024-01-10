#include "Time.h"

std::chrono::time_point<std::chrono::high_resolution_clock> Time::StartTime;
std::chrono::time_point<std::chrono::high_resolution_clock> Time::PreviousTime;
float Time::DeltaTime = 0;
float Time::TimeScale = 0;

void Time::Initialize() {
	StartTime = std::chrono::high_resolution_clock::now();
	PreviousTime = StartTime;
	TimeScale = 1.0f;
}

void Time::Update()
{
	const auto currentTime = std::chrono::high_resolution_clock::now();
	DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - PreviousTime).count() / 1000000.0f;
	DeltaTime *= TimeScale;
	PreviousTime = currentTime;
}