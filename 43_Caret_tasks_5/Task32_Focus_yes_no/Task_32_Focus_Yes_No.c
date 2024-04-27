#include<windows.h>
#include<tchar.h>
#include<stdlib.h>
#include<stdio.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void RandomRectangle(HWND);

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
					TEXT("FOCUS YES OR NO"),
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

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
	HDC hdc;
	
	static int cxClient;
	static int cyClient;

	PAINTSTRUCT ps;

	static int bFlag = 0;
	RECT rect;
	
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			break;
	
	case WM_SETFOCUS:

			CreateCaret(hwnd,NULL,1,2);
			GetClientRect(hwnd,&rect);
			InvalidateRect(hwnd,&rect,TRUE);

			bFlag = 1;
		break;

	case WM_KILLFOCUS:

			DestroyCaret();
			GetClientRect(hwnd,&rect);
			InvalidateRect(hwnd,&rect,TRUE);
			bFlag = 0;
		break;	

	case WM_PAINT:
		
			hdc = BeginPaint(hwnd,&ps);

			GetClientRect(hwnd,&rect);

			SetTextColor(hdc,RGB(255,0,0));
			SetBkColor(hdc,RGB(0,255,0));

			if(bFlag == 1)
			{
				DrawText(hdc , TEXT("Your Windows Have Focus") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{ 

				DrawText(hdc , TEXT("Your Windows Doesn't have focus") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
				
			EndPaint(hwnd,&ps);

				break;
	
	case WM_DESTROY:
		
			PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}