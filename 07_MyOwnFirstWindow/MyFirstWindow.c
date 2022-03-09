#include<windows.h>
#include<stdio.h>
#include"Logger.h"

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

FILE *gFilePointer = NULL;

int WINAPI WinMain(
				HINSTANCE hInstance,
				HINSTANCE HPrevInstance,
				LPSTR lpszCmdLine,
				int iCmdShow)
{
	int iRet;
	MSG msg;
	HWND hwnd;
	WNDCLASSEX wndclass;

	TCHAR szAppName[] = TEXT("MyWindow");

	CreateLog(&gFilePointer);

	//fputs("IN WinMain Function\n",gFilePointer);
	#if  	LOG_WINMAIN
		fputs("IN WinMain Function\n",gFilePointer);
	#endif   //	LOG_WINMAIN


	wndclass.cbSize = sizeof(wndclass);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szAppName;
	wndclass.lpfnWndProc = WndProc;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	//fputs("Before RegisterClassEx\n",gFilePointer);
	#if  	LOG_WINMAIN
		fputs("Before RegisterClassEx\n",gFilePointer);
	#endif   //	LOG_WINMAIN

	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"), szAppName , 0);
		return -1;
	}
	//flushall();
	fflush(gFilePointer);
	//fputs("After RegisterClassEx\n",gFilePointer);
	#if  	LOG_WINMAIN
		fputs("After RegisterClassEx\n",gFilePointer);
	#endif   //	LOG_WINMAIN

	//fputs("Before CreateWindow\n",gFilePointer);	
	#if  	LOG_WINMAIN
		fputs("Before CreateWindow\n",gFilePointer);
	#endif   //	LOG_WINMAIN
			
	hwnd = CreateWindow(
					szAppName,
					TEXT("First Window"),
					WS_OVERLAPPEDWINDOW,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					CW_USEDEFAULT,
					NULL,
					NULL,
					hInstance,
					NULL
					);

	if(hwnd == NULL)
	{
		MessageBox(NULL,TEXT("CreateWindow : FalLURE"), szAppName , 0);
		return -1;
	}

	#if  	LOG_WINMAIN
		fputs("After CreateWindow\n",gFilePointer);
	#endif   //	LOG_WINMAIN
		
	ShowWindow(hwnd,iCmdShow);

	#if  	LOG_WINMAIN
		fputs("After ShowWindow\n",gFilePointer);
	#endif   //	LOG_WINMAIN

	UpdateWindow(hwnd);

	#if  	LOG_WINMAIN
		fputs("After UpdateWindow\n",gFilePointer);
	#endif   //	LOG_WINMAIN

	while((iRet = GetMessage(&msg,NULL,0,0)) != 0)
	{
		#if  	LOG_WINMAIN
					fprintf(gFilePointer,"In While Loop Value of iRet is  %d\n",iRet);
		#endif   //	LOG_WINMAIN

		if(iRet == -1)
		{
			#if  	LOG_WINMAIN
					fprintf(gFilePointer,"In While Loop Value of iRet if -1 %d\n",iRet);
			#endif   //	LOG_WINMAIN

			MessageBox(NULL,TEXT("CreateWindow : FalLURE"), szAppName , 0);
			return -1;
		}
		TranslateMessage(&msg);

		#if  	LOG_WINMAIN
				fputs("After TranslateMessage\n",gFilePointer);
		#endif   //	LOG_WINMAIN

		DispatchMessage(&msg);

		#if  	LOG_WINMAIN
				fputs("After DispatchMessage\n",gFilePointer);
		#endif   //	LOG_WINMAIN
	}

	DestroyLog(&gFilePointer);
	gFilePointer = NULL;
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(
					HWND hwnd,
					UINT uiMessage,
					WPARAM wParam,
					LPARAM lParam
					)
{
	
	//fputs("\t\t\t\tIn WinProFunction\n",gFilePointer);
	//fprintf(gFilePointer,"\t\tValue of uiMessage in WinProc function : uiMessage =  %d\n",uiMessage);
	switch(uiMessage)
	{
	case WM_DESTROY:
			#if  	LOG_WINPROC
					fprintf(gFilePointer,"Value of uiMessage %d\n",uiMessage);
			#endif   //	LOG_WINMAIN
			PostQuitMessage(0);
	case WM_CREATE: 
		MessageBox(NULL,__TEXT("Say Ok to Display Your First windows"),__TEXT("Confirmation"),MB_OK);
	}

	return DefWindowProc(hwnd,uiMessage,wParam,lParam);

}
void CreateLog(FILE **fp)
{
	*fp = fopen("Logger.txt","w");

	if(*fp == NULL)
	{
		printf("File Opening Failed");
		return ;
	}
}
void DestroyLog(FILE **fp)
{
	fclose(*fp);
	*fp = NULL;
}
