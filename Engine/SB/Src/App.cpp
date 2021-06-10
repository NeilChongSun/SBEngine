#include"Precomplied.h"
#include"App.h"

using namespace SB;
using namespace SB::Graphics;
using namespace SB::Input;

void SB::App::ChangeState(const std::string & name)
{
	if (auto iter = mAppStates.find(name); iter != mAppStates.end())
		mNextState = iter->second.get();
}

void SB::App::Run(AppConfig appConfig)
{
	mAppConfig = std::move(appConfig);

	//Setup our application window

	mWindow.Initialize(GetModuleHandle(NULL), mAppConfig.appName.c_str(), mAppConfig.windowWidth, mAppConfig.windowHeight);
	
	//Intialize our application window
	auto handle = mWindow.GetWindowHandle();
	InputSystem::StaticInitialize(handle);

	//Intialize the gramphics systems
	GraphicsSystem::StaticInitialize(handle, false);
	DebugUI::StaticInitialize(handle, false, true);
	SimpleDraw::StaticInitialize();
	//Intialize the starting state
	mCurrentState = mAppStates.begin()->second.get();
	mCurrentState->Initialize();


	mRunning = true;
	while (mRunning&&mWindow.isActive())
	{
		mWindow.ProcessMessage();
		if (!mWindow.isActive())
		{
			Quit();
			continue;
		}
		if (mNextState)
		{
			mCurrentState->Terminate();
			mCurrentState = std::exchange(mNextState, nullptr);
			mCurrentState->Initialize();
		}

		auto inputSystem = InputSystem::Get();
		inputSystem->Update();

		if (inputSystem->IsKeyPressed(KeyCode::ESCAPE))
		{
			Quit();
			continue;
		}

		float deltaTime = 1.0f / 60.0f;
		mCurrentState->Update(deltaTime);

		auto graphicsSystem = GraphicsSystem::Get();
		graphicsSystem->BeginRender();
		mCurrentState->Render();
		DebugUI::BeginRender();
		mCurrentState->DebugUI();
		DebugUI::EndRender();
		graphicsSystem->EndRender();
		//Do game stuff....
	}
	//Terminate engine system
	SimpleDraw::StaticTerminate();
	DebugUI::StaticTerminate();
	GraphicsSystem::StaticTerminate();
	InputSystem::StaticTerminate();
	//Terminate window
	mWindow.Terminate();

}
