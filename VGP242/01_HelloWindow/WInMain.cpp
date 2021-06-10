
#include<SB/Inc/SB.h>

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	SB::MainApp().AddState<SB::AppState>("DummyState");
	SB::MainApp().Run({ "Hello Window",1280 * 720 });
	return 0;
}