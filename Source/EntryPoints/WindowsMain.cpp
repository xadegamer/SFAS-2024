// The main Windows / DirectX Graphics / XInput entry point for Star Applications

#define WIN32_LEAN_AND_MEAN

#include <Windows.h>
#include "Engine/Implementation/DirectX11/DirectX11Graphics.h"
#include "Engine/Implementation/XInput/DirectXInput.h"
#include "Engine/IRenderable.h"
#include "Engine/ITexture.h"
#include "Engine/IShader.h"
#include "Engine/IApplication.h"
#include "UI/UISystem.h"
#include "Game/SoundManager.h"
#include "Engine/Time.h"
#include "Engine/ParticleSystem.h"

const char WindowClassName[] = "Star";
const char WindowTitle[] = "Search for a Star 2024";
const int WindowWidth = 1920 / 1.5;
const int WindowHeight = 1080 / 1.5;
const std::string MainMenuCanvasID = "MainMenuCanvas";

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
IApplication* GetApplication(IGraphics* Graphics, IInput* Input);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	WNDCLASSEX wc;
	HWND hwnd;

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = WindowClassName;
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc))
	{
		MessageBox(NULL, "Window Registration Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	// Added to centre the window on the screen
	int centreScreenX = GetSystemMetrics(SM_CXSCREEN) / 2 - WindowWidth / 2;
	int centreScreenY = GetSystemMetrics(SM_CYSCREEN) / 2 - WindowHeight / 2;

	RECT WindowRect;
	WindowRect.left = centreScreenX;
	WindowRect.top = centreScreenY;
	WindowRect.right = WindowRect.left + WindowWidth;
	WindowRect.bottom = WindowRect.top + WindowHeight;
	AdjustWindowRect (&WindowRect, WS_OVERLAPPEDWINDOW, FALSE);

	hwnd = CreateWindowEx(0, WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW,
		WindowRect.left, WindowRect.top, WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,NULL,NULL, hInstance, NULL);

	//hwnd = CreateWindowEx( WS_EX_CLIENTEDGE, WindowClassName, WindowTitle, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, WindowWidth, WindowHeight, NULL, NULL, hInstance, NULL);

	if (hwnd == NULL)
	{
		MessageBox(NULL, "Window Creation Failed!", "Error!", MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);
	UpdateWindow(hwnd);

	MSG msg;
	msg.message = WM_NULL;
	msg.wParam = -1;
	IGraphics * Graphics = new DirectX11Graphics(hwnd);
	IInput* Input = new DirectXInput();

	UISystem::Init(Graphics, Input);

	IApplication* Application = GetApplication(Graphics, Input);

	SoundManager::Initialize();

	Time::Initialize();

	ParticleSystem::Init(Graphics);

	SoundManager::PlayMusic("MainMenu");

	if (Graphics && Graphics->IsValid() && Application)
	{
		while (msg.message != WM_QUIT && Application->IsValid())
		{
			if (PeekMessage(&msg, 0, 0, 0, PM_REMOVE))
			{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
			}

			Time::Update();
			Input->Update();
			Application->Update();
			Graphics->Update();
			ParticleSystem::Update(Time::GetDeltaTime());
			UISystem::Update(0.0f);
			SoundManager::Update();
		}

		Application->Cleanup();
	}

	if (Application)
	{
		delete Application;
	}

	if (Graphics)
	{
		delete Graphics;
	}

	SoundManager::Clear();

	ParticleSystem::Clear();

	return static_cast<int>(msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_CLOSE:
		DestroyWindow(hwnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hwnd, msg, wParam, lParam);
	}

	return 0;
}
