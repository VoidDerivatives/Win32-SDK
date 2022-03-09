#include<windows.h>

int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PSTR szCmdLineArguments,
		int iCmdShow)

{	
	MessageBox(NULL,TEXT("Hello World"),TEXT("Win32"),0);

	return 0;
}