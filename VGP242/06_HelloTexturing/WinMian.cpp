#include<SB/Inc/SB.h>
#include"GameState.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SB::MainApp().AddState<GameState>("GameState");
	SB::MainApp().Run({ "Hello Cube", 1280, 720 });

	return 0;
}