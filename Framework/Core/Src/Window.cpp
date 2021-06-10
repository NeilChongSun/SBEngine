#include"Precompiled.h"
#include "Window.h"

using namespace SB;
using namespace SB::Core;

LRESULT CALLBACK WinProc(HWND handle, UINT message, WPARAM WParam, LPARAM Iparam)
{
	switch (message)
	{
		case WM_DESTROY:
		{
			PostQuitMessage(0);
			return 0;
		}
	}
	return DefWindowProcA(handle, message, WParam, Iparam);
}

void SB::Core::Window::Initialize(HINSTANCE instance, LPCSTR appName, uint32_t width, uint32_t height)
{
	mInstance = instance;
	mAppName = appName;

	//3 steps to create a window
	//1. Register a "Window Class" with Win32
	//2. Create a window instance of that class type
	//3. Retrieve/dispatch/handle messages for this window

	//Register class
	WNDCLASSEXA classInfo{};
	classInfo.cbSize = sizeof(WNDCLASSEXA);
	classInfo.style = CS_HREDRAW | CS_VREDRAW;
	classInfo.lpfnWndProc = WinProc;
	classInfo.hInstance = instance;
	classInfo.hIcon = LoadIcon(nullptr, IDI_APPLICATION);
	classInfo.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	classInfo.lpszClassName = appName;
	classInfo.hIconSm = LoadIcon(nullptr, IDI_APPLICATION);

	RegisterClassExA(&classInfo);

	//Compute the correct window dimension
	RECT rc = { 0,0,static_cast<LONG>(width),static_cast<LONG>(height) };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

	//Compute window position so it's centered on screen
	const int screenWidth = GetSystemMetrics(SM_CXSCREEN);
	const int screenHeight = GetSystemMetrics(SM_CYSCREEN);
	const int winWidth = rc.right - rc.left;
	const int winHeight = rc.bottom - rc.top;
	const int posX = (screenWidth - winWidth) / 2;
	const int posY = (screenHeight - winHeight) / 2;

	//Create Window
	mWindow = CreateWindowA(appName, appName, WS_OVERLAPPEDWINDOW, posX, posY, winWidth, winHeight, nullptr, nullptr, instance, nullptr);
	ShowWindow(mWindow, SW_SHOWNORMAL);
	SetCursorPos(screenWidth / 2, screenHeight / 2);
	mActive = (mWindow != nullptr);
	

}

void SB::Core::Window::Terminate()
{
	DestroyWindow(mWindow);
	UnregisterClassA(mAppName.c_str(), mInstance);
	mWindow = nullptr;
	mInstance = nullptr;
}

void SB::Core::Window::ProcessMessage()
{
	MSG msg{};
	while (PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);

		if (WM_QUIT == msg.message)
			mActive = false;
	}	
}
