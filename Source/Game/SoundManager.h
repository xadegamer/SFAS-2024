#pragma once

#include "Audio.h"
#include <memory>
#include <map>
#include "TypeHelpers.h"

class SoundManager
{
private:

	static std::unique_ptr<DirectX::AudioEngine> audioEngine;
	static std::map<std::string, std::unique_ptr<DirectX::SoundEffect>> soundEffects;
	static std::map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> soundEffectInstances;
	static std::string currentMusic;

public:
	SoundManager() = delete;
	~SoundManager() = delete;

	static void Initialize();
	static void Update();
			    
	static void LoadSFX(std::string name, std::wstring filePath);
	static void PlayOneShot(std::string name);
			    
	static void PlayMusic(std::string name);
	static void StopMusic(std::string name);
			    
	static void SetVolume(std::string name, float volume);

	static void Clear();
};

