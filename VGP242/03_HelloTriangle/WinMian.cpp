#include<SB/Inc/SB.h>
#include"GameState.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SB::MainApp().AddState<GameState>("RedState");
	SB::MainApp().Run({ "Hello Triangle", 1280, 720 });
	return 0;
}