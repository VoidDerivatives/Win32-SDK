#include<windows.h>
#include<tchar.h>
#include<stdio.h>
#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

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
					TEXT("EDIT BOX"),
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

#define ID_EDIT 1

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		static HWND hwndEdit;
		static int cxClient;
		static int cyClient;

		switch(uiMessage)
		{
			case WM_CREATE:

				hwndEdit = CreateWindow(
										TEXT("Edit"),
										NULL,
										WS_CHILD | WS_VISIBLE | WS_VSCROLL | WS_HSCROLL | WS_BORDER | ES_LEFT 
										| ES_MULTILINE | ES_AUTOHSCROLL | ES_AUTOVSCROLL ,
										0,
										0,
										0,
										0,
										hwnd,
										(HMENU)ID_EDIT,
										(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
										NULL
										);
				return 0;

			case WM_SETFOCUS:

				SetFocus(hwndEdit);	
			
				return 0;
			
			case WM_SIZE:
				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
				MoveWindow(hwndEdit,0,0,cxClient,cyClient,TRUE);	
				ShowWindow(hwndEdit,SW_SHOWNORMAL);
				return 0;
			
			case WM_COMMAND:
				
				if(LOWORD(wParam) == ID_EDIT)
					if(HIWORD(wParam) == EN_ERRSPACE | HIWORD(wParam) == EN_MAXTEXT)
						MessageBox(hwnd,__TEXT("MAX"),__TEXT("Window"),MB_OK);
				
				return 0;		

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
