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
					TEXT("KEY's BEHAVIOUR"),
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
	RECT rect;
	static RECT rect1,rect2,rect3;
	PAINTSTRUCT ps;
	static int cxClient;
	static int cyClient;
	static short bShift , bControl , bCapital;
	TCHAR szText[255];
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			break;

	case WM_KEYDOWN:
			
			hdc = GetDC(hwnd);
			GetClientRect(hwnd,&rect);

			switch(wParam)
			{
				case VK_CAPITAL:

					rect.top =  0;
					rect.left = 0;
					rect.bottom = cyClient / 5;
					rect.right = cxClient;

					InvalidateRect(hwnd,&rect,TRUE);
			
					break;
				
				case VK_NUMLOCK:
					
						rect.top = cyClient / 5;
						rect.left = 0;
						rect.bottom = cyClient / 3;
						rect.right = cxClient;
			
						InvalidateRect(hwnd,&rect,TRUE);

					break;

				case VK_SHIFT:
					bShift = 1;
					break;

				case VK_CONTROL:
					bControl = 1;

				case 65:
					bCapital = 1;		

			}

			ReleaseDC(hwnd,hdc);
			break;			
	
	case WM_LBUTTONDOWN:
			hdc = GetDC(hwnd);

				// shift
				InvalidateRect(hwnd,&rect1,TRUE);



				// control
				InvalidateRect(hwnd,&rect2,TRUE);


				// Captial
				InvalidateRect(hwnd,&rect3,TRUE);


			ReleaseDC(hwnd,hdc);
			break;

	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);
			
			SetTextColor(hdc,RGB(255,0,0));
			SetBkMode(hdc,TRANSPARENT);

			// Caps Lock
			rect.top =  0;
			rect.left = 0;
			rect.bottom = cyClient / 5;
			rect.right = cxClient;
			if(GetKeyState(VK_CAPITAL) & 1)
			{
				DrawText(hdc , TEXT("Caps Lock :     | ON |") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				DrawText(hdc , TEXT("Caps Lock :     | OFF |") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			// Number Lock
			rect.top = cyClient / 5;
			rect.left = 0;
			rect.bottom = cyClient / 3;
			rect.right = cxClient;
			if(GetKeyState(VK_NUMLOCK) & 1)
			{
				DrawText(hdc , TEXT("Num Lock :       | ON |") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				DrawText(hdc , TEXT("Num Lock :       | OFF |") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}


			// Shift 
			rect1.top = cyClient / 3;
			rect1.left = 0;
			rect1.bottom = cyClient / 2;
			rect1.right = cxClient;

			if(GetKeyState(VK_SHIFT) & 128)
			{
				DrawText(hdc , TEXT("Shift Bit :      | Press |") , -1 , &rect1 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				DrawText(hdc , TEXT("Shift Bit :      | Not Press |") , -1 , &rect1 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}


			// control

			rect2.top = cyClient / 2;
			rect2.left = 0;
			rect2.bottom = cyClient - (cyClient/3);
			rect2.right = cxClient;
			if(GetKeyState(VK_CONTROL) & 128)
			{
				DrawText(hdc , TEXT("CTRL Bit :      | Press |") , -1 , &rect2 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				DrawText(hdc , TEXT("CTRL Bit :      | Not Press |") , -1 , &rect2 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}

			// Capital A

			rect3.top = cyClient - (cyClient / 3);
			rect3.left = 0;
			rect3.bottom = cyClient;
			rect3.right = cxClient;

			
			if(GetKeyState(65) & 128)
			{
				DrawText(hdc , TEXT("Captial A :      | Press |") , -1 , &rect3 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			else
			{
				DrawText(hdc , TEXT("Captial A :      | Not Press |") , -1 , &rect3 , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			}
			
			EndPaint(hwnd,&ps);
		
				break;
	
	case WM_DESTROY:
		
			PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}