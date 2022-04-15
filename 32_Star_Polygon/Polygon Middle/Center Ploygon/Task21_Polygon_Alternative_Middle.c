#include<windows.h>
#include<stdio.h>

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
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"),szAppName,MB_OK);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("Middle Star"),
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
		MessageBox(hwnd,TEXT("CreateWindow : FAlLURE"),szAppName,MB_OK);
		return -1;	
	}

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	while((bRet = GetMessage(&msg,NULL,0,0)) != 0)
	{
		if(bRet == -1)
		{
			MessageBox(NULL,TEXT("CreateWindow : FAlLURE"), szAppName , MB_OK);
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
	PAINTSTRUCT ps;
	RECT rect;

	static int cxClient;
	static int cyClient;
	HBRUSH hBrush;
	static int iColorCount;

	int iX1;
	int iY1;

	POINT apt[5];
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			break;
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			if(iColorCount == 0)
				hBrush = CreateSolidBrush(RGB(230,32,194));
			else if(iColorCount == 1)
				hBrush = CreateSolidBrush(RGB(111,42,237));
			else if(iColorCount == 2)
				hBrush = CreateSolidBrush(RGB(237,187,35));
			else if(iColorCount == 3)
				hBrush = CreateSolidBrush(RGB(10,199,237));
			else
				hBrush = CreateSolidBrush(RGB(230,43,124));

			SelectObject(hdc,hBrush);

			iX1 = cxClient / 3;
			iY1 = (cyClient / 2) + 100;

			apt[0].x = iX1;
			apt[0].y = iY1;

			iX1 =  cxClient / 2 ;
			iY1 = (cyClient / 2) - 150 ;

			apt[1].x = iX1;
			apt[1].y = iY1;

			iX1 = cxClient - (cxClient / 3);	

			apt[2].x = iX1;
			apt[2].y = apt[0].y;

			iY1 = cyClient / 2 - 100;

			apt[3].x = apt[0].x;
			apt[3].y = iY1;
			apt[4].x = apt[2].x;
			apt[4].y = apt[3].y;

			rect.top = 0;
			rect.left = 0;
			rect.bottom = apt[1].y;
			rect.right = cxClient;
	
			SetBkColor(hdc,RGB(128,128,255));
			DrawText(hdc,TEXT("My Star having Multiple Color With Alternative Mode"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			//SetPolyFillMode(hdc,WINDING);
			Polygon(hdc,apt, MY_ARRAY_SIZE(apt));

			iColorCount++;
			if(iColorCount == 5)
				iColorCount = 0; 

			EndPaint(hwnd,&ps);
			DeleteObject(hBrush);
			
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}