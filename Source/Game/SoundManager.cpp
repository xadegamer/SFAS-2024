#include "SoundManager.h"
#include <iostream>

std::unique_ptr<DirectX::AudioEngine> SoundManager::audioEngine = nullptr;
std::map<std::string, std::unique_ptr<DirectX::SoundEffect>> SoundManager::soundEffects;
std::map<std::string, std::unique_ptr<DirectX::SoundEffectInstance>> SoundManager::soundEffectInstances;
std::string SoundManager::currentMusic = "";

void SoundManager::Initialize()
{
	HRESULT hr = CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	if (FAILED(hr))
	{
		throw std::exception("Failed to initialize COM library.");
	}

	audioEngine = std::make_unique<DirectX::AudioEngine>();

	DirectX::AUDIO_ENGINE_FLAGS eflags = DirectX::AudioEngine_Default;

#ifdef _DEBUG
	eflags |= DirectX::AudioEngine_Debug;
#endif

	audioEngine = std::make_unique<DirectX::AudioEngine>(eflags);

	LoadSFX("MainMenu", L"Resource\\Audio\\puzzle2.wav");
	LoadSFX("GameMenu", L"Resource\\Audio\\puzzle1.wav");
	LoadSFX("Button_Hover", L"Resource\\Audio\\Hover.wav");
	LoadSFX("Button_Click", L"Resource\\Audio\\Click.wav");

	std::cout << "Sound Handler Initialized" << std::endl;
	audioEngine->SetMasterVolume(0.1f);
	std::cout << "Volume: " << audioEngine->GetMasterVolume() << std::endl;
}

void SoundManager::Update()
{
	audioEngine->Update();
}

void SoundManager::LoadSFX(std::string name, std::wstring filePath)
{
	std::unique_ptr<DirectX::SoundEffect> soundEffect = std::make_unique<DirectX::SoundEffect>(audioEngine.get(), filePath.c_str());
	soundEffects[name] = std::move(soundEffect);
}

void SoundManager::PlayOneShot(std::string name)
{
	if (!soundEffects[name].get())
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffects[name]->Play();
}

void SoundManager::PlayMusic(std::string name)
{
	if (currentMusic != "")
	{
		soundEffectInstances[currentMusic]->Stop();
	}

	if (!soundEffects[name])
	{
		std::cout << "Music not found" << std::endl;
		return;
	}

	soundEffectInstances[name] = soundEffects[name]->CreateInstance();

	soundEffectInstances[name]->Play(true);

	currentMusic = name;
}

void SoundManager::StopMusic(std::string name)
{
	if (!soundEffectInstances[name])
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffectInstances[name]->Stop();
}

void SoundManager::SetVolume(std::string name, float volume)
{
	if (!soundEffectInstances[name])
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffectInstances[name]->SetVolume(volume);
}

void SoundManager::Clear()
{
	audioEngine->Suspend();
	audioEngine.release();
}
