#include<windows.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define TimerId1		1

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc1(HWND,UINT,WPARAM,LPARAM);

LRESULT CALLBACK WindowProcessMessages(HWND,UINT,WPARAM,LPARAM);
#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#define GWL_HINSTANCE       (-6) // Winuser.h

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
					TEXT("RADIO BUTTON"),
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

int iTime;
LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		static HWND hwndButton[2];
		static TCHAR *cArr[3] = {"RED","GREEN","YELLOW"};
		static int cxClient;
		static int cyClient;
		RECT rect;
		TCHAR szText[255];
		static int bFlag = 0;
		HBRUSH hBrush;
		HFONT hFont;
		int iCounter;

		switch(uiMessage)
		{	
			case WM_CREATE:
			
				for(iCounter = 0;iCounter<3;iCounter++)
				{
					hwndButton[iCounter] =  CreateWindow(
						TEXT("button"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTORADIOBUTTON | BS_NOTIFY,
						0,
						0,
						0,
						0,
						hwnd,
						(HMENU)iCounter,
						(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
						NULL
						);
				}
		
				
				break;
			
			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
				
				for(iCounter = 0;iCounter<3;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],0,(cyClient/3)+iCounter*50,80,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);	
				}

				break;
			
			case WM_COMMAND:

				hdc = GetDC(hwnd);
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:

							if(LOWORD(wParam) == 0)
							{
								//SendMessage((HWND)lParam,BM_SETCHECK,1,0);
								bFlag = 1;
								//SendMessage(hwndButton[1],BM_SETCHECK,0,0);
								//SendMessage(hwndButton[2],BM_SETCHECK,0,0);
							}
							else if(LOWORD(wParam) == 1)
							{
								//SendMessage((HWND)lParam,BM_SETCHECK,1,0);
								bFlag = 2;
								//SendMessage(hwndButton[0],BM_SETCHECK,0,0);
								//SendMessage(hwndButton[2],BM_SETCHECK,0,0);
							}
							else if(LOWORD(wParam) == 2)
							{
								//SendMessage((HWND)lParam,BM_SETCHECK,1,0);
								bFlag = 3;
								//SendMessage(hwndButton[0],BM_SETCHECK,0,0);
								//SendMessage(hwndButton[1],BM_SETCHECK,0,0);
							}
							if(bFlag == 1 || bFlag == 2 || bFlag == 3)
								InvalidateRect(hwnd,NULL,TRUE);

							break;
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
	
				GetClientRect(hwnd,&rect);
			
				
				for(iCounter = 0;iCounter<3;iCounter++)
				{
						if(bFlag == 1 && iCounter == 0)
							hBrush = CreateSolidBrush(RGB(255,0,0));
						else if(bFlag == 2 && iCounter == 1)
							hBrush = CreateSolidBrush(RGB(0,255,0));
						else if(bFlag == 3 && iCounter == 2)
							hBrush = CreateSolidBrush(RGB(255,242,0));
						else
							hBrush = CreateSolidBrush(RGB(255,255,255));
					
					SelectObject(hdc,hBrush);
					Rectangle(hdc,cxClient/2,(cyClient/3)+iCounter*50,cxClient/2+80,(cyClient/3)+((iCounter+1)*50));	
					DeleteObject(hBrush);
				}

				hBrush = CreateSolidBrush(RGB(255,255,255));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,cxClient/2+35,(cyClient/3)+3*50,cxClient/2+45,cyClient);
				DeleteObject(hBrush);
				EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
