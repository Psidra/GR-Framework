#include "DetectMemoryLeak.h"
#include "Application.h"

#define _DEBUGMODE 1;	//	0=Non-debug mode, 1=Debug mode

// this ensures that _CrtDumpMemoryLeaks() is called after main() and right before program terminates
struct AtExit
{
	~AtExit() { _CrtDumpMemoryLeaks(); }
} doAtExit;

int main( void )
{
	Application &app = Application::GetInstance();
	app.Init();
	app.Run();
	app.Exit();
}