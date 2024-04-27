#include<windows.h>
#include<tchar.h>

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
					TEXT("CHECK BOX BUTTON"),
					WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
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
#define STRING_COUNT 11

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		static HWND hwndButton[STRING_COUNT - 1];
		static BOOL Flag[STRING_COUNT]={0};
		static TCHAR *cArr[STRING_COUNT] = {"RED","GREEN","BLUE","PINK","YELLOW","PURPLE","BROWN","ORANGE","GREY","VIOLET","SKYBLUE"};
		static int cxClient;
		static int cyClient;
		RECT rect;
		static int bFlag = 0;
		HBRUSH hBrush;
		HFONT hFont;
		int iCounter;
		static int iCounterCalculation;

		switch(uiMessage)
		{	
			case WM_CREATE:

				for(iCounter = 0;iCounter<STRING_COUNT;iCounter++)
				{
					hwndButton[iCounter] =  CreateWindow(
						TEXT("button"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_AUTOCHECKBOX | BS_NOTIFY,
						0,
						0,
						0,
						0,
						hwnd,
						(HMENU)iCounter,
						(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
						NULL
						);

					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
				}
		

				break;
			
			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
				
				for(iCounter = 0;iCounter<STRING_COUNT;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],0,iCounter*51,90,51,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
				}
				
				for(iCounter = 0;iCounter<11;iCounter++)
				{
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);	
				}

				break;
			
			case WM_COMMAND:

				hdc = GetDC(hwnd);
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:
							iCounterCalculation = LOWORD(wParam);
							
							if(LOWORD(wParam) == 0)
							{
								if(Flag[0] == TRUE)
									Flag[0] = FALSE;
								else
									Flag[0] = TRUE;
							}
							else if(LOWORD(wParam) == 1)
							{
								if(Flag[1] == TRUE)
									Flag[1] = FALSE;
								else
									Flag[1] = TRUE;
							}
							else if(LOWORD(wParam) == 2)
							{
								if(Flag[2] == TRUE)
									Flag[2] = FALSE;
								else
									Flag[2] = TRUE;	
							}
							else if(LOWORD(wParam) == 3)
							{
								if(Flag[3] == TRUE)
									Flag[3] = FALSE;
								else
									Flag[3] = TRUE;
							}
							else if(LOWORD(wParam) == 4)
							{
								if(Flag[4] == TRUE)
									Flag[4] = FALSE;
								else
									Flag[4] = TRUE;
							}
							else if(LOWORD(wParam) == 5)
							{
								if(Flag[5] == TRUE)
									Flag[5] = FALSE;
								else
									Flag[5] = TRUE;
							}
							else if(LOWORD(wParam) == 6)
							{
								if(Flag[6] == TRUE)
									Flag[6] = FALSE;
								else
									Flag[6] = TRUE;
							}
							else if(LOWORD(wParam) == 7)
							{
								if(Flag[7] == TRUE)
									Flag[7] = FALSE;
								else
									Flag[7] = TRUE;	
							}
							else if(LOWORD(wParam) == 8)
							{
								if(Flag[8] == TRUE)
									Flag[8] = FALSE;
								else
									Flag[8] = TRUE;
							}
							else if(LOWORD(wParam) == 9)
							{
								if(Flag[9] == TRUE)
									Flag[9] = FALSE;
								else
									Flag[9] = TRUE;
							}
							else if(LOWORD(wParam) == 10)
							{
								if(Flag[10] == TRUE)
									Flag[10] = FALSE;
								else
									Flag[10] = TRUE;
							}
						
							InvalidateRect(hwnd,NULL,FALSE);

							break;
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
			

					if(iCounterCalculation == 0 && Flag[0] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(255,0,0));   // RED
					}
					else if(iCounterCalculation == 1 && Flag[1] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(0,255,0));   // GREEN
					}
					else if(iCounterCalculation==2 && Flag[2] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(0,0,255));   // BLUE
					}
					else if(iCounterCalculation==3 && Flag[3] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(253,22,226));   // PINK
					}
					else if(iCounterCalculation==4 && Flag[4] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(253,249,8));  // YELLOW
					}
					else if(iCounterCalculation==5 && Flag[5] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(195,30,253));   // PURPLE
					}
					else if(iCounterCalculation==6 && Flag[6] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(119,67,66));    // BROWN
					}
					else if(iCounterCalculation==7 && Flag[7] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(247,130,36));  // Orange
					}
					else if(iCounterCalculation==8 && Flag[8] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(128,128,128));   // Gray
					}
					else if(iCounterCalculation==9 && Flag[9] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(0,255,0));
					}
					else if(iCounterCalculation==10 && Flag[10] == TRUE)
					{
						hBrush = CreateSolidBrush(RGB(115,251,253));    //Sky blue
					}
					else
					{
						hBrush = CreateSolidBrush(RGB(0,0,0));
					}

					SelectObject(hdc,hBrush);
					
					Rectangle(hdc,55,iCounterCalculation*51,cxClient,(iCounterCalculation+1)*51);
				
				DeleteObject(hBrush);
				EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
