#pragma once
namespace SB::Core {
	class Window
	{
	public:
		Window() = default;

		void Initialize(
			HINSTANCE instance,  LPCSTR appName, uint32_t width, uint32_t height);

		void Terminate();
		void ProcessMessage();

		HWND GetWindowHandle() const { return mWindow; }

		bool isActive() const { return mActive; }
	private:
		HINSTANCE mInstance = nullptr;
		HWND mWindow = nullptr;
		std::string mAppName;
		bool mActive = false;
	};
}