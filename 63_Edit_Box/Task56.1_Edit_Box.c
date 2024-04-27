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
		HBRUSH hBrush;
		HFONT hFont;
		RECT rect;

		static HWND hwndButtonStatic1,hwndButtonStatic2;
		static HWND hwndButtonEdit1,hwndButtonEdit2,hwndButton1;

		static HWND hwndButton[9];
		static TCHAR *cArr[4] = {TEXT("Enter the Decimal number :"),TEXT("Hex number"),TEXT("Binary number"),
								TEXT("Octal number")};
	
		static int cxClient;
		static int cyClient;
		
		static TCHAR buffer[50];
		TCHAR szText[255]={TEXT(255)};
		static TCHAR stringDest[30]={TEXT("")},stringSource[30];
		
		static int number;
		int iCounter,remainder,count=0;
		int iLength,num;
		int a[100];

		unsigned char c,j,n;
		int i;
		unsigned char ans=128;

		switch(uiMessage)
		{	
			case WM_CREATE:

				for(iCounter = 0;iCounter<1;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
						TEXT("Edit"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY | ES_NUMBER | SS_BLACKFRAME 
						| WS_BORDER,
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

				for(;iCounter<2;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
						TEXT("Edit"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY | ES_LOWERCASE  | SS_BLACKFRAME 
						| ES_READONLY | WS_BORDER,
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
				for(;iCounter<6;iCounter++)
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
				for(;iCounter<7;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
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

				for(;iCounter<8;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
						TEXT("Edit"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY | ES_LOWERCASE | SS_BLACKFRAME 
						| ES_READONLY | WS_BORDER,
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

				for(;iCounter<9;iCounter++)
				{
					hwndButton[iCounter] = CreateWindow(
						TEXT("Edit"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY | ES_LOWERCASE | SS_BLACKFRAME 
						| ES_READONLY | WS_BORDER,
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

				for(iCounter = 0;iCounter<1;iCounter++)
				{
				MoveWindow(hwndButton[iCounter],cxClient/2,cyClient/4,200,20,TRUE);
				ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);	
				}

				// Edit Box2
				for(;iCounter<2;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],cxClient/2,cyClient/3+50,200,20,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);	
				}

				for(;iCounter<6;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],cxClient/4,cyClient/4+((iCounter-2)*100),170,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArr[iCounter-2]);
				
				}


				// Button
				for(;iCounter<7;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],40,cyClient/3-10,160,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],"CONVERT");		
				}
					
				for(;iCounter<8;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],cxClient/2,cyClient/2+50,200,20,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);	
				}	

				for(;iCounter<9;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],cxClient/2,cyClient/2+150,200,20,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);	
				}	

				break;
			case WM_COMMAND:

				hdc = GetDC(hwnd);
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:
							if((HWND)lParam == hwndButton[6]) //Submit
							{	
								memset(stringDest,0, 30*sizeof(TCHAR));
								InvalidateRect(hwnd,NULL,TRUE);
							}
							break;
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);
					
					// All Area
				hBrush = CreateSolidBrush(RGB(181,181,181));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,5,5,cxClient - 5 ,cyClient - 5);
				DeleteObject(hBrush);
				
				iLength = SendMessage(hwndButton[0],EM_GETLINE,0,(LPARAM)szText);
				szText[iLength]='\0';
				sscanf(szText, "%d", &number);
					
					for(iCounter=0;number!=0;iCounter++)
					{
						remainder=number%16;
						number=number/16;
						a[iCounter]=remainder;
						count++;
					}
				    for(iCounter=count-1;iCounter>=0;iCounter--)
					{
						switch(a[iCounter])
							{
						case 10:
								strcpy(stringSource,TEXT("a"));
								strcat(stringDest,"a");
							  break;
						case 11:
								strcpy(stringSource,TEXT("b"));
								strcat(stringDest,TEXT("b"));
							 break;
						case 12:
								strcpy(stringSource,TEXT("c"));
								strcat(stringDest,TEXT("c"));
							 break;
						case 13:
								strcpy(stringSource,TEXT("d"));
								strcat(stringDest,TEXT("d"));
							 break;
						case 14:
								strcpy(stringSource,TEXT("e"));
								strcat(stringDest,stringSource);
							 break;
						case 15:
								strcpy(stringSource,TEXT("f"));
								strcat(stringDest,"f");
							break;
						default:
							sprintf(stringSource, "%d", a[iCounter]);
							strcat(stringDest,stringSource);
						}
					}
			
							SendMessage(hwndButton[1],EM_SETMODIFY,0,(LPARAM)stringDest);
							SetWindowText(hwndButton[1],stringDest);

							
					// Binary Display
							memset(szText,255, 255*sizeof(TCHAR));
							iLength = SendMessage(hwndButton[0],EM_GETLINE,0,(LPARAM)szText);
							szText[iLength]='\0';
							sscanf(szText, "%d", &number);
							num = number;
						
						if(num != 0)
						{
							memset(stringDest,0, 30*sizeof(TCHAR));
							memset(stringSource,0, 30*sizeof(TCHAR));
							for(iCounter=1;iCounter<=16;iCounter++)
							{
								buffer[iCounter]=num%2;
								if(buffer[iCounter] == 0)
									strcpy(stringSource,TEXT("0 "));
								else
									strcpy(stringSource,TEXT("1 "));
								strcat(stringDest,stringSource);
								num=num/2;
							}
						}	

						SendMessage(hwndButton[7],EM_SETMODIFY,0,(LPARAM)buffer);
						SetWindowText(hwndButton[7],stringDest);	
					
					// Octal			
					memset(stringDest,0, 30*sizeof(TCHAR));
					memset(stringSource,0, 30*sizeof(TCHAR));
					iLength = SendMessage(hwndButton[0],EM_GETLINE,0,(LPARAM)szText);
					szText[iLength]='\0';
					sscanf(szText, "%d", &number);
					
					for(iCounter=0;number!=0;iCounter++)
					{
						remainder=number%8;
						number=number/8;
						a[iCounter]=remainder;
						count++;
					}
				    for(iCounter=count-1;iCounter>=0;iCounter--)
					{
						switch(a[iCounter])
							{
						case 10:
								strcpy(stringSource,TEXT("a"));
								strcat(stringDest,"a");
							  break;
						case 11:
								strcpy(stringSource,TEXT("b"));
								strcat(stringDest,TEXT("b"));
							 break;
						case 12:
								strcpy(stringSource,TEXT("c"));
								strcat(stringDest,TEXT("c"));
							 break;
						case 13:
								strcpy(stringSource,TEXT("d"));
								strcat(stringDest,TEXT("d"));
							 break;
						case 14:
								strcpy(stringSource,TEXT("e"));
								strcat(stringDest,stringSource);
							 break;
						case 15:
								strcpy(stringSource,TEXT("f"));
								strcat(stringDest,"f");
							break;
						default:
							sprintf(stringSource, "%d", a[iCounter]);
							strcat(stringDest,stringSource);
						}
					}
				SendMessage(hwndButton[8],EM_SETMODIFY,0,(LPARAM)buffer);
						SetWindowText(hwndButton[8],stringDest);
					EndPaint(hwnd,&ps);
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
