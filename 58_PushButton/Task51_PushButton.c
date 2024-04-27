#include<windows.h>
#include<strsafe.h>
#include<string.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
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
	wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
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
					TEXT("PUSH BUTTON"),
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
		PAINTSTRUCT ps;
		HDC hdc;
		static HWND hwndButton[2];
		static int cxClient;
		static int cyClient;
		RECT rect;
		static int bFlag = 0;
		HBRUSH hBrush;
		TCHAR buffer[50];

		switch(uiMessage)
		{	
			case WM_CREATE:
				hwndButton[0] =  CreateWindow(
					TEXT("BUTTON"),
					TEXT(""),
					WS_TABSTOP | WS_VISIBLE | WS_CHILD |BS_PUSHBUTTON | BS_NOTIFY,
					0,
					0,
					0,
					0,
					hwnd,
					NULL,
					(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
					NULL
					);

				hwndButton[1] =  CreateWindow(
					TEXT("BUTTON"),
					TEXT(""),
					WS_TABSTOP | WS_VISIBLE | WS_CHILD |BS_PUSHBUTTON | BS_NOTIFY,
					0,
					0,
					0,
					0,
					hwnd,
					NULL,
					(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
					NULL
					);

				ShowWindow(hwndButton[0],SW_HIDE);
				ShowWindow(hwndButton[1],SW_HIDE);

				break;
			
			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
				
				MoveWindow(hwndButton[0],0,0,80,50,TRUE);
				ShowWindow(hwndButton[0],SW_SHOWNORMAL);
				
				MoveWindow(hwndButton[1],85,0,120,50,TRUE);
				ShowWindow(hwndButton[1],SW_SHOWNORMAL);
				
				break;
			
			case WM_COMMAND:

				switch(HIWORD(wParam))
				{
					case BN_CLICKED:

						if((HWND)lParam == hwndButton[0])
						{
							GetWindowText(hwndButton[0],buffer,sizeof(buffer));
							if(!strcmp(buffer,"RED"))
								bFlag = 1;
							else
								bFlag = 0;
						}

						if((HWND)lParam == hwndButton[1])
						{
							GetWindowText(hwndButton[1],buffer,sizeof(buffer));
							if(!strcmp(buffer,"RANDOM COLOR"))
								bFlag = 2;
							else
								bFlag = 0;
						}

							InvalidateRect(hwnd,NULL,TRUE);
							break;
				}
				
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
				//SetTextColor(hdc,RGB(255,255,255));
				GetClientRect(hwnd,&rect);
	
				if(bFlag == 1)
				{
					hBrush = CreateSolidBrush(RGB(255 ,0, 0));
					SetWindowText(hwndButton[0],"BLACK");
				}
				else if(bFlag == 2)
				{
					hBrush = CreateSolidBrush(RGB(rand()%256,rand()%256,rand()%256));
					SetWindowText(hwndButton[1],"BLACK");
				}
				else
				{
					hBrush = CreateSolidBrush(RGB(0 ,0, 0));
					SetWindowText(hwndButton[0],"RED");	
					SetWindowText(hwndButton[1],"RANDOM COLOR");
				}
				SelectObject(hdc,hBrush);
				FillRect(hdc,&rect,hBrush);

				if(bFlag == 1)
					DrawText(hdc,TEXT("Background Color is RED"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				else if(bFlag == 2)
					DrawText(hdc,TEXT("Background Color is RANDOM"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				else
					DrawText(hdc,TEXT("Background Color is BLACK"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);



				DeleteObject(hBrush);
		

				EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

