#include<windows.h>
#include<stdio.h>
#include<tchar.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT   600

int WINAPI WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrecInstance,
					LPSTR     pszCmdLine,
					int      iCmdShow
					)
{
	MSG msg;
	WNDCLASSEX wndclass;
	BOOL bRet;
	HWND hwnd;

	int cxScreen;
	int cyScreen;

	int x;
	int y;

	TCHAR szAppName[] = TEXT("CenterWindow");

	wndclass.cbSize = sizeof(wndclass);

	wndclass.hInstance = hInstance;
	wndclass.lpfnWndProc = WndProc;
	wndclass.lpszClassName = szAppName;

	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.hIcon = LoadIcon(NULL,IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"),szAppName,0);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("MOVE AND LINE USE"),
					WS_OVERLAPPEDWINDOW,
					x,
					y,
					WINDOW_WIDTH,
					WINDOW_HEIGHT,
					NULL,
					NULL,
					hInstance,
					NULL
					);

	if(hwnd == NULL)
	{
		MessageBox(hwnd,TEXT("CreateWindow : FAlLURE"),szAppName,0);
		return -1;	
	}

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	while((bRet = GetMessage(&msg,NULL,0,0)) != 0)
	{
		if(bRet == -1)
		{
			MessageBox(NULL,TEXT("CreateWindow : FAlLURE"), szAppName , 0);
			return -1;
		}
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hdc;
	int iCounter1 = 0;
	int iCounter2 = 0;
	static int cxClient;
	static int cyClient;
	
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			for(iCounter2 = cyClient; iCounter1 < cxClient && iCounter2 >= 0; iCounter1+=100)
			{
				MoveToEx(hdc,iCounter1,0,NULL);	
				LineTo(hdc,cxClient,iCounter2);
				iCounter2-=100;
			}	

			iCounter1 = 100;
			for(iCounter2 = cxClient; iCounter1 < cyClient && iCounter2 >= 0; iCounter1+=100)
			{
				iCounter2-=100;
				MoveToEx(hdc,0,iCounter1,NULL);	
				LineTo(hdc,iCounter2,cyClient);
			}

			for(iCounter1 = cxClient , iCounter2 = cyClient; iCounter1 >= 0 && iCounter2 >= 0; iCounter1-=100)
			{
				MoveToEx(hdc,iCounter1,0,NULL);	
				LineTo(hdc,0,iCounter2);
				iCounter2-=100;
			}	

			for(iCounter1 = 100 , iCounter2 = 100 ; iCounter2 < cyClient && iCounter1 < cxClient; iCounter1+=100)
			{
				MoveToEx(hdc,cxClient,iCounter1,NULL);	
				LineTo(hdc,iCounter2,cyClient);
				iCounter2+=100;
			}	

			EndPaint(hwnd,&ps);
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}