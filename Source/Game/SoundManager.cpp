#include "SoundManager.h"
#include <iostream>

std::unique_ptr<DirectX::AudioEngine> SoundManager::audioEngine = nullptr;
std::map<Name, std::unique_ptr<DirectX::SoundEffect>> SoundManager::soundEffects;
std::map<Name, std::unique_ptr<DirectX::SoundEffectInstance>> SoundManager::soundEffectInstances;

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

	std::cout << "Sound Handler Initialized" << std::endl;
	audioEngine->SetMasterVolume(0.1f);
	std::cout << "Volume: " << audioEngine->GetMasterVolume() << std::endl;
}

void SoundManager::Update()
{
	audioEngine->Update();
}

void SoundManager::LoadSFX(Name name, std::wstring filePath)
{
	std::unique_ptr<DirectX::SoundEffect> soundEffect = std::make_unique<DirectX::SoundEffect>(audioEngine.get(), filePath.c_str());
	soundEffects[name] = std::move(soundEffect);
}

void SoundManager::PlayOneShot(Name name)
{
	if (!soundEffects[name].get())
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffects[name]->Play();
}

void SoundManager::PlayMusic(Name name)
{
	if (!soundEffects[name])
	{
		std::cout << "Music not found" << std::endl;
		return;
	}

	soundEffectInstances[name] = soundEffects[name]->CreateInstance();

	soundEffectInstances[name]->Play(true);
}

void SoundManager::StopMusic(Name name)
{
	if (!soundEffectInstances[name])
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffectInstances[name]->Stop();
}

void SoundManager::SetVolume(Name name, float volume)
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
