#include<SB/Inc/SB.h>
#include"TestStates.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SB::MainApp().AddState<RedState>("RedState");
	SB::MainApp().AddState<BlueState>("BlueState");

	SB::MainApp().Run({ "Hello DirectX", 1280, 720 });
	return 0;
}