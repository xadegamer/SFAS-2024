#include "SoundSystem.h"
#include <iostream>

SoundSystem::~SoundSystem()
{
	audioEngine->Suspend();
	audioEngine.release();
}

void SoundSystem::Initialize()
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

	LoadSFX("MainTheme", L"Sounds/music.wav");
	LoadSFX("Pickup", L"Sounds/pickup.wav");
	LoadSFX("Shoot", L"Sounds/shoot.wav");
	LoadSFX("Scream", L"Sounds/scream.wav");

	std::cout << "Sound Handler Initialized" << std::endl;
	audioEngine->SetMasterVolume(0.1f);
	std::cout << "Volume: " << audioEngine->GetMasterVolume() << std::endl;

}

void SoundSystem::Update()
{
	audioEngine->Update();
}

void SoundSystem::LoadSFX(Name name, std::wstring filePath)
{
	std::unique_ptr<DirectX::SoundEffect> soundEffect = std::make_unique<DirectX::SoundEffect>(audioEngine.get(), filePath.c_str());
	soundEffects[name] = std::move(soundEffect);
}

void SoundSystem::PlayOneShot(Name name)
{
	if (!soundEffects[name].get())
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffects[name]->Play();
}

void SoundSystem::PlayMusic(Name name)
{
	if (!soundEffects[name])
	{
		std::cout << "Music not found" << std::endl;
		return;
	}

	soundEffectInstances[name] = soundEffects[name]->CreateInstance();

	soundEffectInstances[name]->Play(true);
}

void SoundSystem::SetVolume(Name name, float volume)
{
	if (!soundEffectInstances[name])
	{
		std::cout << "Sound effect not found" << std::endl;
		return;
	}

	soundEffectInstances[name]->SetVolume(volume);
}