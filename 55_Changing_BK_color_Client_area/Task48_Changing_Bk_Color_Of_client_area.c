#include<windows.h>
#include<strsafe.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define TimerId1		1
#define TimerId2		2
#define TimerId3		3
#define TimerId4		4

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc1(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc2(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc3(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc4(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

int WINAPI WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrecInstance,
					LPSTR     pszCmdLine,
					int      iCmdShow
					)
{
	MSG msg;
	BOOL bRet;
	HWND hwnd;
	int cxScreen;
	int cyScreen;
	WNDCLASSEX wndclass;
	int y;
	TCHAR szAppName[] = TEXT("CenterWindow");
	int x;

	// parent
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
					TEXT("BACKGROUND COLOR"),
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
			MessageBox(NULL,TEXT("CreateWindow : FAILURE"), szAppName , 0);
			return -1;
		}
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}

	return (int)msg.wParam;
}

int cxClient;
int cyClient;

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;		
		
		switch(uiMessage)
		{	

			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
			
				break;
			case WM_CREATE:
				SetTimer(hwnd,TimerId1,1000,(TIMERPROC)TimerProc1);
				SetTimer(hwnd,TimerId2,1000,(TIMERPROC)TimerProc2);
				SetTimer(hwnd,TimerId3,1000,(TIMERPROC)TimerProc3);
				SetTimer(hwnd,TimerId4,1000,(TIMERPROC)TimerProc4);
				break;
			case WM_PAINT:
				
				hdc = BeginPaint(hwnd,&ps);

				EndPaint(hwnd,&ps);
				break;
			case WM_DESTROY:
				
				PostQuitMessage(0);
				KillTimer(hwnd,TimerId1);
				KillTimer(hwnd,TimerId2);
				KillTimer(hwnd,TimerId3);
				KillTimer(hwnd,TimerId4);
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
void CALLBACK TimerProc1(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);

	rect.top = 0;
	rect.left = 0;
	rect.bottom = cyClient/2;
	rect.right = cxClient/2;
	
	hBrush = CreateSolidBrush(RGB(rand()%255 , rand()%255 , rand()%255));

	FillRect(hdc,&rect,hBrush);

	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}
void CALLBACK TimerProc2(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);

	rect.top = 0;
	rect.left = cxClient/2;
	rect.bottom = cyClient/2;
	rect.right = cxClient;
	
	hBrush = CreateSolidBrush(RGB(rand()%255 , rand()%255 , rand()%255));

	FillRect(hdc,&rect,hBrush);

	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}
void CALLBACK TimerProc3(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);

	rect.top = cyClient/2;
	rect.left = 0;
	rect.bottom = cyClient;
	rect.right = cxClient/2;
	
	hBrush = CreateSolidBrush(RGB(rand()%255 , rand()%255 , rand()%255));

	FillRect(hdc,&rect,hBrush);

	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}
void CALLBACK TimerProc4(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	HDC hdc;
	RECT rect;
	HBRUSH hBrush;

	hdc = GetDC(hwnd);

	rect.top = cyClient/2;
	rect.left = cxClient/2;
	rect.bottom = cyClient;
	rect.right = cxClient;
	
	hBrush = CreateSolidBrush(RGB(rand()%255 , rand()%255 , rand()%255));

	FillRect(hdc,&rect,hBrush);

	ReleaseDC(hwnd,hdc);
	DeleteObject(hBrush);
}