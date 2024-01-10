#pragma once

#include "Audio.h"
#include <memory>
#include <map>
#include "TypeHelpers.h"

class SoundManager
{
private:
	static  std::unique_ptr<DirectX::AudioEngine> audioEngine;
	static  std::map<Name, std::unique_ptr<DirectX::SoundEffect>> soundEffects;
	static  std::map<Name, std::unique_ptr<DirectX::SoundEffectInstance>> soundEffectInstances;

public:
	SoundManager() = delete;
	~SoundManager() = delete;

	static void Initialize();
	static void Update();
			    
	static void LoadSFX(Name name, std::wstring filePath);
	static void PlayOneShot(Name name);
			    
	static void PlayMusic(Name name);
	static void StopMusic(Name name);
			    
	static void SetVolume(Name name, float volume);

	static void Clear();
};

