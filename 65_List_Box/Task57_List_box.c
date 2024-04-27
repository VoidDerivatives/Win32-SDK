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
					TEXT("LIST BOX"),
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
		static HWND hwndListBox[2];
		static HWND hwndButton[2];
		static TCHAR *cArr[8] = {"Assembly","C","C++","STL","Tool Chain","UNIX","Win32-SDK","Pre-Placement"};
		static TCHAR *cButtonArr[2] = {">","CLEAR"};
		TCHAR String[50];
		static int cxClient;
		static int cyClient;

		static int iCounter;
		static int arr[10];

		RECT rect;


		

		switch(uiMessage)
		{	
			case WM_CREATE:
			
					for(iCounter = 0;iCounter<2;iCounter++)
					{
						hwndListBox[iCounter] =  CreateWindow(
						TEXT("listbox"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | LBS_NOTIFY | LBS_MULTIPLESEL | WS_BORDER,
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

					for(iCounter = 0;iCounter<2;iCounter++)
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
					}
		

				break;
			
			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
					
				
					MoveWindow(hwndListBox[0],cxClient/2 - 200,(cyClient/3),100,200,TRUE);
					ShowWindow(hwndListBox[0],SW_SHOWNORMAL);
					

					MoveWindow(hwndListBox[1],cxClient/2 + 120,(cyClient/3),100,200,TRUE);
					ShowWindow(hwndListBox[1],SW_SHOWNORMAL);
					
					for(iCounter = 0; iCounter < 8 ; iCounter++)
						SendMessage(hwndListBox[0],LB_ADDSTRING,0,(LPARAM)cArr[iCounter]);	
				
					// BUTTON
					for(iCounter=0;iCounter<2;iCounter++)
					{
						MoveWindow(hwndButton[iCounter],cxClient/2 - 50,cyClient/3+(iCounter*100),130,50,TRUE);
						ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
						SetWindowText(hwndButton[iCounter],cButtonArr[iCounter]);
					}
				break;
			
			case WM_COMMAND:

				hdc = GetDC(hwnd);
				
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:

							memset(arr,-1, 10*sizeof(int));
							if(LOWORD(wParam) == 0) // <
							{
								SendMessage(hwndListBox[0],LB_GETSELITEMS,8,(LPARAM)arr);
								SendMessage(hwndListBox[1],LB_RESETCONTENT,0,0);		
								for(iCounter = 0; iCounter < 8; iCounter++)
								{
									if(arr[iCounter]!=-1)
									{
										SendMessage(hwndListBox[0],LB_GETTEXT,(WPARAM)arr[iCounter],(LPARAM)String);	
										SendMessage(hwndListBox[1],LB_ADDSTRING,0,(LPARAM)String);
										//SendMessage(hwndListBox[0],LB_DELETESTRING,0,(LPARAM)arr[iCounter]);
									}
								}

								/*for(iCounter = 0; iCounter < 8; iCounter++)
								{
									if(arr[iCounter]==-1)
										break;
									SendMessage(hwndListBox[0],LB_DELETESTRING,(LPARAM)arr[iCounter],0);
								}*/
							}
							if(LOWORD(wParam) == 1)  // >
							{
								SendMessage(hwndListBox[1],LB_RESETCONTENT,0,0);
								SendMessage(hwndListBox[0],LB_RESETCONTENT,0,0);
								for(iCounter = 0; iCounter < 8 ; iCounter++)
									SendMessage(hwndListBox[0],LB_ADDSTRING,0,(LPARAM)cArr[iCounter]);	
							}
							
							break;
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
				
				rect.top = 0;
				rect.left = 0;
				rect.bottom = cyClient/2;
				rect.right = cxClient;

				DrawText(hdc,"FILL and CLEAR The Right 'LIST BOX'",-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				
				EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
