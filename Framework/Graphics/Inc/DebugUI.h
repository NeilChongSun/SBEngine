#pragma once
namespace SB::Graphics::DebugUI {

	enum class Theme
	{
		Classic,
		Dark,
		Light
	};

	void StaticInitialize(HWND window, bool docking = false, bool multiViewport=false);
	void StaticTerminate();
	void SetTheme(Theme theme);
	void BeginRender();
	void EndRender();
} //namespace SB::Graphics::DebugUI