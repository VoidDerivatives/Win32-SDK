#include<windows.h>
#include<tchar.h>

int MyMessageBox(HWND , UINT , const TCHAR * , const TCHAR * , ...);

int WINAPI WinMain(
		HINSTANCE hInstance,
		HINSTANCE hPrevInstance,
		PSTR szCmdLineArguments,
		int iCmdShow
		)

{	
	MyMessageBox(NULL,MB_OK | MB_ICONERROR,TEXT("Win32"),TEXT("Addition of %d and %d is %d"),10,20,30);

	MyMessageBox(NULL,MB_OK | MB_ICONERROR,TEXT("Win32"),TEXT("%s got %lf percentage int %d standard"),TEXT("Aishwarya"),99.99,12);

	return 0;
}

#define MY_ARRAY_SIZE(x) ( sizeof(x) / sizeof(x[0]) )

int CDECL MyMessageBox(HWND hwnd , UINT uiType , const TCHAR *pszCaption , const TCHAR *pszFormat , ...)
{
	TCHAR szBuffer[256];

	va_list pArgs = NULL;

	va_start(pArgs,pszFormat);

	_vsntprintf(szBuffer,MY_ARRAY_SIZE(szBuffer),pszFormat,pArgs);

	return MessageBox(hwnd , szBuffer, pszCaption , MB_OK);
}