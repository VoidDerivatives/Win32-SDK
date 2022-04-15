#include<windows.h>
#include<stdio.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

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
	wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hIconSm = LoadIcon(NULL,IDI_APPLICATION);

	if(!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL,TEXT("RegisterClassEx : FAlLURE"),szAppName,0);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("COMBO USE"),
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

			MessageBox(NULL,TEXT("CreateWindow : FAlL;/?URE"), szAppName , 0);
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
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	HPEN hPen;

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

			hPen = CreatePen(PS_SOLID , 1 , RGB(255,0,0));
			SelectObject(hdc,hPen);

			SelectObject(hdc,GetStockObject(BLACK_BRUSH));
			
			Rectangle(hdc,cxClient/8,cyClient/8,7*cxClient/8,7*cyClient/8);

			LineTo(hdc,cxClient,cyClient);

			MoveToEx(hdc,0,cyClient,NULL);

			LineTo(hdc,cxClient,0);

			Ellipse(hdc,cxClient/8,cyClient/8,7*cxClient/8,7*cyClient/8);

			RoundRect(hdc,cxClient/4,cyClient/4,3*cxClient/4,3*cyClient/4,cxClient/4,cyClient/4);

			rect.top = cyClient/8 ;
			rect.left = cxClient/8 ;
			rect.bottom = 7*cyClient/8;
			rect.right = 7*cxClient/8;

			SetTextColor(hdc,RGB(120,5,28));
			SetBkColor(hdc,RGB(255,197,58));
			DrawText(hdc,TEXT("UNIX NEVER Says , Please !!!"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			EndPaint(hwnd,&ps);
			DeleteObject(hPen);
			
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}