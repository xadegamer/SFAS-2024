#pragma once
#include <chrono>

class Time {
public:
	static void Initialize();

	static void Update();

	static inline float GetDeltaTime() { return DeltaTime; }
	static inline void SetTimeScale(float scale) { TimeScale = scale; }
	static inline float GetTimeScale() { return TimeScale; }

private:
	static std::chrono::time_point<std::chrono::high_resolution_clock> StartTime;
	static std::chrono::time_point<std::chrono::high_resolution_clock> PreviousTime;
	static float DeltaTime;
	static float TimeScale;

};