#include<SB/Inc/SB.h>
#include"GameState.h"
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SB::MainApp().AddState<Heart>("Heart");
	SB::MainApp().AddState<House>("House");
	SB::MainApp().AddState<Triforce>("Triforce");
	//SB::MainApp().Run({ "Hello DirectX", 1280, 720 });
	SB::MainApp().Run({ "Hello Shaps", 1080, 1080});

	return 0;
}