#pragma once

#include "Audio.h"
#include <memory>
#include <map>
#include "TypeHelpers.h"

class SoundManager
{
private:

	static std::unique_ptr<DirectX::AudioEngine> AudioEngine;
	static std::map<std::string, std::unique_ptr<DirectX::SoundEffect>> SoundEffects;
	static std::map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> SoundEffectInstances;
	static std::string CurrentMusic;
	static float MusicVolume;
	static float SFXVolume;

public:
	SoundManager() = delete;
	~SoundManager() = delete;

	static void Initialize();
	static void Update();
			    
	static void LoadSFX(std::string name, std::wstring filePath);
	static void PlayOneShot(std::string name, bool randomPitch = false);
			    
	static void PlayMusic(std::string name);
	static void StopMusic(std::string name);

	static void PlayAudio(std::string name, bool loop);
	static void StopAudio(std::string name);
			    
	static void SetGlobalVolume(float volume);

	static void Pause();
	static void Resume();

	static void Clear();
};

