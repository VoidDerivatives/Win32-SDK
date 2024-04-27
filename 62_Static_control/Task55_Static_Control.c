#include<windows.h>
#include<tchar.h>

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
					TEXT("STATIC USE BUTTON"),
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

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		static HWND hwndButton[12];
		static TCHAR *cArr[12] = {"MALE","FEMALE","TRANSGENDER","AADHAR","PAN","LICENSE",
								"SUMBIT","CLEAR","CANCLE","RADIO BUTTON","CHECK BOX","BUTTONS"};
		static int cxClient;
		static int cyClient;
		RECT rect;
		TCHAR buffer[50];
		TCHAR szText[255];
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

					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
				}

				for(;iCounter<6;iCounter++)
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

				for(;iCounter<9;iCounter++)
				{
					hwndButton[iCounter] =  CreateWindow(
						TEXT("button"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY,
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

				for(;iCounter<12;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
							TEXT("static"),
							NULL,
							WS_TABSTOP | WS_VISIBLE | WS_CHILD | SS_WHITEFRAME | SS_RIGHT,
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
				
				// Radio Button
				for(iCounter = 0;iCounter<3;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],((cxClient/2)/2)+135*iCounter,20,130,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);	
				}

				// Check box
				for(;iCounter<6;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],((cxClient/2)/2)+135*(iCounter-3),100,130,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);	
				}

				// Button
				for(;iCounter<9;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],((cxClient/2)/2)+135*(iCounter-6),180,130,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);

					if(iCounter == 7)
						EnableWindow(hwndButton[iCounter],FALSE);

				}
				rect.top = 50*(iCounter-8);
				for(;iCounter<12;iCounter++)
				{		
					MoveWindow(hwndButton[iCounter],40,rect.top + 70 * (iCounter-9),130,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter]);		
				}

			case WM_COMMAND:

				hdc = GetDC(hwnd);
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:

							if(LOWORD(wParam) == 0) // MALE
							{
								SendMessage(hwndButton[1],BM_SETCHECK,0,0);
								SendMessage(hwndButton[2],BM_SETCHECK,0,0);
							}
							else if(LOWORD(wParam) == 1)  // FEMALE
							{
								SendMessage(hwndButton[0],BM_SETCHECK,0,0);
								SendMessage(hwndButton[2],BM_SETCHECK,0,0);
							}
							else if(LOWORD(wParam) == 2)   // TRANSGENDER
							{
								SendMessage(hwndButton[0],BM_SETCHECK,0,0);
								SendMessage(hwndButton[1],BM_SETCHECK,0,0);
							}

							
							// BUTTON
							if((HWND)lParam == hwndButton[6]) //Submit
							{	
								InvalidateRect(hwnd,NULL,TRUE);
							}

							if((HWND)lParam == hwndButton[7]) // CLEAR
							{
								for(iCounter = 0;iCounter<6;iCounter++)
								{
									SendMessage(hwndButton[iCounter],BM_SETCHECK,0,0);
								}
								InvalidateRect(hwnd,NULL,TRUE);
								EnableWindow(hwndButton[7],FALSE);
							}
							if((HWND)lParam == hwndButton[8]) // CANCLE
							{	
								iCounter = MessageBox(NULL,TEXT("Do You want to Close the Application"),TEXT("Printing"),MB_YESNO);
								if(iCounter == IDYES)
								{
									SendMessage(hwnd,WM_DESTROY,0,0);
								}	
							}
							for(iCounter = 0;iCounter<6;iCounter++)
							{
								if(SendMessage(hwndButton[iCounter],BM_GETCHECK,0,0))
								{
									EnableWindow(hwndButton[7],TRUE);
									break;
								}
								if(iCounter == 5 )	
									EnableWindow(hwndButton[7],FALSE);
							}
							break;
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
	
				GetClientRect(hwnd,&rect);

				// All Area
				hBrush = CreateSolidBrush(RGB(181,181,181));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,5,5,cxClient - 5 ,cyClient - 5);
				DeleteObject(hBrush);

				hBrush = CreateSolidBrush(RGB(138,138,138));
				SelectObject(hdc,hBrush);

				// Down Area
				Rectangle(hdc,10,cyClient/2,cxClient -10,cyClient - 10);
				
				// Up Area
				Rectangle(hdc,10,10,cxClient -10,(cyClient/2) - 10);
				DeleteObject(hBrush);

				// Printing
				hFont = CreateFont(25,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont);

				rect.top = cyClient/2+10;
				rect.left = 20;
				rect.bottom = cyClient - 10;
				rect.right = cxClient -10;
				
				SetTextColor(hdc,RGB(41,47,133));
				SetBkMode(hdc,TRANSPARENT);
				DrawText(hdc,"RADIO BUTTON OUTPUT",-1,&rect,DT_LEFT );

				rect.top = rect.top+60;
				rect.left = rect.left;
				rect.bottom = rect.bottom;
				rect.right = rect.right;
				DrawText(hdc,"CHECK BOX OUTPUT",-1,&rect,DT_LEFT );
				DeleteObject(hFont);

				SetTextColor(hdc,RGB(0,0,0));
				for(iCounter = 0;iCounter<6;iCounter++)
				{
					GetWindowText(hwndButton[iCounter],buffer,sizeof(buffer));
					if(SendMessage(hwndButton[iCounter],BM_GETCHECK,0,0))
					{
					
						if(iCounter == 0 || iCounter == 1 || iCounter == 2)
						{
							rect.top = cyClient/2+40;
							rect.left = rect.left;
							rect.bottom = rect.bottom;
							rect.right = rect.right;
							DrawText(hdc,buffer,-1,&rect,DT_LEFT );
							iCounter = 2;
							rect.top = rect.top+30;
						}
						else
						{
							rect.top = rect.top+30;
							rect.left = rect.left;
							rect.bottom = rect.bottom;
							rect.right = rect.right;
							DrawText(hdc,buffer,-1,&rect,DT_LEFT );
						}
					}
					
				}

				EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
