#include<windows.h>
#include<math.h>
#include<tchar.h>
#include<stdlib.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void RandomRectangle(HWND);

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT   600
#define TWO_PIE 2.0*3.14156

int g_cxClient;
int g_cyClient;

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
					TEXT("RANDOM RECTANGLE"),
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

	while(1)
	{
		if(PeekMessage(&msg,NULL,0,0,PM_REMOVE))
		{
			if(msg.message == WM_QUIT)
				break;
		
			TranslateMessage(&msg);
			DispatchMessage(&msg);	
		}
		else
			RandomRectangle(hwnd);
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
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			g_cxClient = LOWORD(lParam);
			g_cyClient = HIWORD(lParam);

			break;

	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);
			
			EndPaint(hwnd,&ps);
		
				break;
	
	case WM_DESTROY:
		
			PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

// Randome Rectangle
void RandomRectangle(HWND hwnd)
{	
	HDC hdc;
	HBRUSH hBrush;
	RECT rect;

	if(g_cxClient == 0 || g_cyClient == 0)
		return;

	SetRect(&rect,rand()%g_cxClient,rand()%g_cyClient,rand()%g_cxClient,rand()%g_cyClient);	
	hBrush = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));

	hdc = GetDC(hwnd);

	FillRect(hdc,&rect,hBrush);


	SetTextColor(hdc,RGB(120,5,28));
	DrawText(hdc,TEXT("SCREEN SEVER"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	


	ReleaseDC(hwnd,hdc);

	DeleteObject(hBrush);

	Sleep(500);

}
