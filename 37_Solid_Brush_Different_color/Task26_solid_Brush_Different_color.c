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
					TEXT("SOLID BRUSH DIFFERENT COLOR"),
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
	HBRUSH hBrush;

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

			hPen = CreatePen(PS_DASH , 1 , RGB(251,21,193));
			SelectObject(hdc,hPen);

			hBrush = CreateSolidBrush(RGB(71,9,1));
			SelectObject(hdc,hBrush);

			Rectangle(hdc,cxClient/8,cyClient/8,7*cxClient/8,7*cyClient/8);

			DeleteObject(hPen);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0,255,0));
			SelectObject(hdc,hBrush);

			hPen = CreatePen(PS_DASHDOT , 1 , RGB(221,30,2));
			SelectObject(hdc,hPen);

			LineTo(hdc,cxClient,cyClient);
			MoveToEx(hdc,0,cyClient,NULL);

			DeleteObject(hPen);
			DeleteObject(hBrush);

			hPen = CreatePen(PS_DASHDOT , 1 , RGB(221,30,2));
			SelectObject(hdc,hPen);
			
			hBrush = CreateSolidBrush(RGB(1,71,68));
			SelectObject(hdc,hBrush);

			LineTo(hdc,cxClient,0);
			Ellipse(hdc,cxClient/8,cyClient/8,7*cxClient/8,7*cyClient/8);

			DeleteObject(hPen);
			DeleteObject(hBrush);

			hBrush = CreateSolidBrush(RGB(0,255,0));
			SelectObject(hdc,hBrush);

			hPen = CreatePen(PS_DOT , 1 , RGB(71,9,1));
			SelectObject(hdc,hPen);	

			RoundRect(hdc,cxClient/4,cyClient/4,3*cxClient/4,3*cyClient/4,cxClient/4,cyClient/4);
			DeleteObject(hPen);
			DeleteObject(hBrush);
			

			rect.top = cyClient/8 ;
			rect.left = cxClient/8 ;
			rect.bottom = 7*cyClient/8;
			rect.right = 7*cxClient/8;

			SetTextColor(hdc,RGB(120,5,28));
			SetBkColor(hdc,RGB(255,197,58));
			DrawText(hdc,TEXT("GIVE ME WINGS , LET ME FLY , BELIVE In Me , LET ME TOUCH THE SKY!!!"),-1,
					&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			EndPaint(hwnd,&ps);
			
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}