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

public:
	SoundManager() = delete;
	~SoundManager() = delete;

	static void Initialize();
	static void Update();
			    
	static void LoadSFX(std::string name, std::wstring filePath);
	static void PlayOneShot(std::string name);
			    
	static void PlayMusic(std::string name);
	static void StopMusic(std::string name);
			    
	static void SetMusicVolume(float volume);
	static void SetSoundEffectVolume(std::string name, float volume);

	static void Clear();
};

