#pragma once
#include"AppState.h"
namespace SB {

	struct AppConfig
	{
		std::string appName = "SB";
		uint32_t windowWidth = 1280;
		uint32_t windowHeight = 720;
	};


	class App
	{
	public:
		template<class StateType, class=std::void_t<std::is_base_of<AppState,StateType>>>
		void AddState(std::string name)
		{
			mAppStates.emplace(std::move(name), std::make_unique <StateType>());
		}

		void ChangeState(const std::string& name);

		void Run(AppConfig appConfig);
		void Quit() { mRunning = false; }
	private:
		AppConfig mAppConfig;
		Core::Window mWindow;
		bool mRunning = true;

		std::map <std::string, std::unique_ptr<AppState>>mAppStates;
		AppState* mCurrentState = nullptr;
		AppState* mNextState = nullptr;

	};

}//namespace SBchong921130@gmail.com