#include "SoundManager.h"
#include <iostream>

std::unique_ptr<DirectX::AudioEngine> SoundManager::AudioEngine = nullptr;
std::map<std::string, std::unique_ptr<DirectX::SoundEffect>> SoundManager::SoundEffects;
std::map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> SoundManager::SoundEffectInstances;
std::string SoundManager::CurrentMusic = "";
float SoundManager::MusicVolume = 0.5f;
float SoundManager::SFXVolume = 1.0f;

void SoundManager::Initialize()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		throw std::exception("Failed to initialize COM library.");
	}

	AudioEngine = std::make_unique<DirectX::AudioEngine>();

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif

	AudioEngine = std::make_unique<DirectX::AudioEngine>(eflags);

	LoadSFX("MainMenu", L"Resource\\Audio\\puzzle2.wav");
	LoadSFX("GameMenu", L"Resource\\Audio\\puzzle1.wav");
	LoadSFX("Button_Hover", L"Resource\\Audio\\Hover.wav");
	LoadSFX("Button_Click", L"Resource\\Audio\\Click.wav");

	std::cout << "Sound Handler Initialized" << std::endl;
	//AudioEngine->SetMasterVolume(0.5f);
	std::cout << "Volume: " << AudioEngine->GetMasterVolume() << std::endl;
}

void SoundManager::Update()
{
	AudioEngine->Update();
}

void SoundManager::LoadSFX(std::string name, std::wstring filePath)
{
	std::unique_ptr<DirectX::SoundEffect> soundEffect = std::make_unique<DirectX::SoundEffect>(AudioEngine.get(), filePath.c_str());
	SoundEffects[name] = std::move(soundEffect);
}

void SoundManager::PlayOneShot(std::string name, bool randomPitch)
{
	if (!SoundEffects[name].get())
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	float randomPitchValue = randomPitch ? RAND_FLOAT (-0.5f, 0.5f) : 0.0f;
	SoundEffects[name]->Play(SFXVolume, randomPitchValue,0);
}

void SoundManager::PlayMusic(std::string name)
{
	if (CurrentMusic != "")
	{
		SoundEffectInstances[CurrentMusic]->Stop();
	}

	if (!SoundEffects[name])
	{
		std::cout << "Music not found" << std::endl;
		return;
	}

	SoundEffectInstances[name] = SoundEffects[name]->CreateInstance();
	SoundEffectInstances[name]->SetVolume(MusicVolume);
	SoundEffectInstances[name]->Play(true);

	CurrentMusic = name;
}

void SoundManager::StopMusic(std::string name)
{
	if (!SoundEffectInstances[name])
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	SoundEffectInstances[name]->Stop();
}

void SoundManager::SetGlobalVolume(float volume)
{
	AudioEngine->SetMasterVolume(volume);
}

void SoundManager::Pause()
{
	AudioEngine->Suspend();
}

void SoundManager::Resume()
{
	AudioEngine->Resume();
}

void SoundManager::Clear()
{
	AudioEngine->Suspend();
	AudioEngine.release();
}
