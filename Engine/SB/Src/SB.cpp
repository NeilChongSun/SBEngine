#include "Precomplied.h"
#include "SB.h"
namespace { SB::App sApp; }

SB::App& SB::MainApp()
{
	return sApp;
}
