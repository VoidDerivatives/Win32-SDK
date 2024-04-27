#include<windows.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WndProcChild(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#define GWL_HINSTANCE       (-6) // Winuser.h

HWND hwndWindow;

struct Info 
{
	TCHAR szTextName[255];
	TCHAR szTextEmail[255];
	TCHAR szTextPhone[255];
	TCHAR szGender[10];
	BOOL bFavBatch[4];
	TCHAR *pszPrevBatch[255];
	int arr[10];
}obj;

WNDCLASSEX wndclass,wndclassChild;
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

	wndclassChild = wndclass;
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
					TEXT("ALL CHILD WINDOW"),
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
HWND hwndListBox;


LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		HBRUSH hBrush;
		static int cxClient;
		static int cyClient;

		static HWND hwndButtonRC[7]; // R = Radio C = Check
		static HWND hwndButtonstatic[6];
		static HWND hwndButtonEdit[3];
		static HWND hwndButton[2];
		//TCHAR String[50];
		//static HWND hwndWindow;
		TCHAR szAppName[] =TEXT("CHILD WINDOW");

		static TCHAR *cArr[9] = {TEXT("ENTER YOUR DETAILS"),TEXT("Name"),TEXT("Email Id"),TEXT("Phone No"),TEXT("Previous Batch's")};
		static TCHAR *cArrRadio[2] = {TEXT("Male"),TEXT("Female")};
		static TCHAR *cArrCheck[5] = {TEXT("SELECT YOUR FAVORITE BATCH"),TEXT("C"),TEXT("C++"),TEXT("UNIX"),TEXT("ASSEMBLY")};
		static TCHAR *cArrListBox[9] = {TEXT("Assembly"),TEXT("C"),TEXT("C++"),TEXT("STL"),TEXT("Tool Chain"),TEXT("UNIX"),TEXT("Win32-SDK"),
								TEXT("Pre-Placement"),TEXT("SELECT YOUR PREVIOUS Batch's")};
		static TCHAR *cArrButton[2]={TEXT("SUBMIT"),TEXT("CLEAR")};
		int iCounter;
		

		switch(uiMessage)
		{	
			case WM_CREATE:
				
				obj.szTextName[0] = TEXT(255);
				obj.szTextEmail[0] = TEXT(255);
				obj.szTextPhone[0] = TEXT(255);
				obj.szGender[0] = TEXT(255);
				memset(obj.bFavBatch,0, 4*sizeof(BOOL));
				memset(obj.arr,-1, 10*sizeof(int));

				// Radio Button
				for(iCounter = 0;iCounter<2;iCounter++)
				{
					hwndButtonRC[iCounter] =  CreateWindow(
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

				// Check Box
				for(;iCounter<6;iCounter++)
				{
					hwndButtonRC[iCounter] =  CreateWindow(
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
				}
				
				hwndButtonRC[iCounter] = CreateWindow(
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

				//	Static
				for(iCounter = 0;iCounter<6;iCounter++)
				{
					hwndButtonstatic[iCounter] = CreateWindow(
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


				// Edit Box
				for(iCounter = 0;iCounter<3;iCounter++)
				{
					hwndButtonEdit[iCounter] = CreateWindow(
						TEXT("Edit"),
						NULL,
						WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY  | ES_LOWERCASE 
						| SS_BLACKFRAME  | WS_BORDER,
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

						// List Box
						hwndListBox =  CreateWindow(
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
					
				for(iCounter=0;iCounter<2;iCounter++)
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

				// Window
					wndclassChild = wndclass;
					wndclassChild.style = CS_HREDRAW | CS_VREDRAW;
					//wndclassChild.hIcon = NULL;
					wndclassChild.hCursor = LoadCursor(NULL,IDC_ARROW);
					wndclassChild.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
					//wndclassChild.hIconSm = NULL;					
					wndclassChild.lpszClassName = TEXT("MY CHILD");
					wndclassChild.lpfnWndProc = WndProcChild;
				if(!RegisterClassEx(&wndclassChild))
				{
					MessageBox(NULL,TEXT("RegisterClassEx : FAlLURE"),szAppName,0);
					return -1;
				} 
				hwndWindow = CreateWindow(
								TEXT("MY CHILD"),
								NULL,
								WS_POPUPWINDOW | WS_CHILD,
								0,
								0,
								0,
								0,
								hwnd,
								NULL,
								(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
								NULL
								);

				if(hwndWindow == NULL)
				{
					MessageBox(hwnd,TEXT("CreateWindow"),szAppName,0);
					return -1;	
				}
	

				break;
			
			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
				
				// Radio Button
				for(iCounter = 0;iCounter<2;iCounter++)
				{
					MoveWindow(hwndButtonRC[iCounter],cxClient/2+(iCounter)*100,cyClient/10,100,50,TRUE);
					ShowWindow(hwndButtonRC[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButtonRC[iCounter],cArrRadio[iCounter]);	
				}

				// Check Box
				for(;iCounter<6;iCounter++)
				{
					MoveWindow(hwndButtonRC[iCounter],10,(cyClient/3+(iCounter-2)*51)+30,120,51,TRUE);
					ShowWindow(hwndButtonRC[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButtonRC[iCounter],cArrCheck[iCounter-1]);	
				}
				
				// static
				MoveWindow(hwndButtonRC[iCounter],10 ,cyClient/3+5,250,50,TRUE);
				ShowWindow(hwndButtonRC[iCounter],SW_SHOWNORMAL);
				SetWindowText(hwndButtonRC[iCounter],cArrCheck[0]);

				// Static

				MoveWindow(hwndButtonstatic[0],cxClient/2 - 50 ,20,200,50,TRUE);
				ShowWindow(hwndButtonstatic[0],SW_SHOWNORMAL);
				SetWindowText(hwndButtonstatic[0],cArr[0]);
			
				for(iCounter = 1;iCounter<4;iCounter++)
				{		
					MoveWindow(hwndButtonstatic[iCounter],15,cyClient/10+(iCounter-1)*50,130,50,TRUE);
					ShowWindow(hwndButtonstatic[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButtonstatic[iCounter],cArr[iCounter]);		
				}		
				
				// Edit Box
				for(iCounter=0;iCounter<3;iCounter++)
				{
					MoveWindow(hwndButtonEdit[iCounter],150,cyClient/10+iCounter*50,200,20,TRUE);
					ShowWindow(hwndButtonEdit[iCounter],SW_SHOWNORMAL);	
				}

				// List Box
				MoveWindow(hwndListBox,cxClient/2,cyClient/3+30,100,200,TRUE);
				ShowWindow(hwndListBox,SW_SHOWNORMAL);


				for(iCounter = 0; iCounter < 8 ; iCounter++)
					SendMessage(hwndListBox,LB_ADDSTRING,0,(LPARAM)cArrListBox[iCounter]);	
				
				// static
				MoveWindow(hwndButtonstatic[5],cxClient/2 ,cyClient/3+5,250,50,TRUE);
				ShowWindow(hwndButtonstatic[5],SW_SHOWNORMAL);
				SetWindowText(hwndButtonstatic[5],cArrListBox[iCounter]);

				for(iCounter=0 ;iCounter<2;iCounter++)
				{
					MoveWindow(hwndButton[iCounter],cxClient/4+iCounter*140,cyClient/2+150,130,50,TRUE);
					ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
					SetWindowText(hwndButton[iCounter],cArrButton[iCounter]);
				}
				
				
				break;
			
			case WM_LBUTTONDOWN:

            	ShowWindow(hwndWindow,SW_HIDE);
         	break;

			case WM_COMMAND:

				hdc = GetDC(hwnd);
				
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:

					if((HWND)lParam == hwndButton[1]) // CLEAR
					{
						ShowWindow(hwndWindow,SW_HIDE);
						for(iCounter = 0;iCounter<2;iCounter++)
						{
							SendMessage(hwndButtonRC[iCounter],BM_SETCHECK,0,0);
						}

						// Check Box
						for(;iCounter<6;iCounter++)
						{
							SendMessage(hwndButtonRC[iCounter],BM_SETCHECK,0,0);
						}

						SendMessage(hwndListBox,LB_RESETCONTENT,0,0);
						for(iCounter = 0; iCounter < 8 ; iCounter++)
							SendMessage(hwndListBox,LB_ADDSTRING,0,(LPARAM)cArrListBox[iCounter]);	
						
						
						memset(obj.szTextName,0, 255*sizeof(TCHAR));
						memset(obj.szTextEmail,0, 255*sizeof(TCHAR));
						memset(obj.szTextPhone,0, 255*sizeof(TCHAR));
						memset(obj.bFavBatch,0, 4*sizeof(BOOL));
						memset(obj.arr,-1, 10*sizeof(int));

						memset(obj.szTextName,-1, sizeof(TCHAR));
						memset(obj.szTextEmail,-1, sizeof(TCHAR));
						memset(obj.szTextPhone,-1, sizeof(TCHAR));
				
						SetWindowText(hwndButtonEdit[0],TEXT(""));
						SetWindowText(hwndButtonEdit[1],TEXT(""));
						SetWindowText(hwndButtonEdit[2],TEXT(""));		
					}
					if((HWND)lParam == hwndButton[0]) // SUBMIT
					{	

						memset(obj.szGender,-1, 10*sizeof(TCHAR));
						SendMessage(hwndButtonEdit[0],EM_GETLINE,0,(LPARAM)obj.szTextName);
						SendMessage(hwndButtonEdit[1],EM_GETLINE,0,(LPARAM)obj.szTextEmail);
						SendMessage(hwndButtonEdit[2],EM_GETLINE,0,(LPARAM)obj.szTextPhone);

						
						if(SendMessage(hwndButtonRC[0],BM_GETSTATE,0,0))
						{
							strcpy(obj.szGender,cArrRadio[0]);
						}
						if(SendMessage(hwndButtonRC[1],BM_GETSTATE,0,0))
						{
							strcpy(obj.szGender,cArrRadio[1]);
						}


						//obj.bFavBatch
						// Check Box
						for(iCounter = 2;iCounter<6;iCounter++)
						{
							if(SendMessage(hwndButtonRC[iCounter],BM_GETSTATE,0,0))
							{
								obj.bFavBatch[iCounter-2] = 1;
							}
						}

						SendMessage(hwndListBox,LB_GETSELITEMS,8,(LPARAM)obj.arr);	
						
						MoveWindow(hwndWindow,cxClient+100,cyClient-100,300,300,TRUE);
						ShowWindow(hwndWindow,SW_SHOWNORMAL);
						UpdateWindow(hwndWindow);
						
					}		
					break;
							
				}
				ReleaseDC(hwnd,hdc);
			
				break;

			case WM_PAINT:

				hdc = BeginPaint(hwnd,&ps);
				
				GetClientRect(hwnd,&rect);

				// All Area
				hBrush = CreateSolidBrush(RGB(204,204,204));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,5,5,cxClient - 5 ,cyClient - 5);
				DeleteObject(hBrush);

				MoveToEx(hdc,0,cyClient/3,NULL);	
				LineTo(hdc,cxClient,cyClient/3);

				EndPaint(hwnd,&ps);
				
				
				break;
	

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
LRESULT CALLBACK WndProcChild(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		int iCounter;
		static TCHAR stringDest[30]={TEXT("")} , stringSource[30];
		static int cxClient;
		static int cyClient;
		HPEN hPen;
		
		memset(stringDest,0, 30*sizeof(TCHAR));
		
		memset(stringSource,0, 30*sizeof(TCHAR));
		switch(uiMessage)
		{
			case WM_SIZE:
				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);

				break;
			
			case WM_PAINT:
				hdc = BeginPaint(hwnd,&ps);
				GetClientRect(hwndWindow,&rect);

				SetTextColor(hdc,RGB(255,0,0));
				SetBkMode(hdc,TRANSPARENT);
			
				if(obj.szTextName[0] != -1)
				{
					sprintf(stringDest,TEXT("Name :"));
					
					strcpy(stringSource,obj.szTextName);
					strcat(stringDest,stringSource);

					DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT | DT_SINGLELINE);
				}


				rect.top = rect.top + 16; 
				
				if(obj.szTextEmail[0] != -1)
				{
					sprintf(stringDest,TEXT("Email :"));
					
					strcpy(stringSource,obj.szTextEmail);
					strcat(stringDest,stringSource);
					DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT | DT_SINGLELINE);
				}

				rect.top = rect.top + 16; 
				if(obj.szTextPhone[0] != -1)
				{
					sprintf(stringDest,TEXT("Phone No:"));
					
					strcpy(stringSource,obj.szTextPhone);
					strcat(stringDest,stringSource);
					DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT| DT_SINGLELINE);
				}

				SetTextColor(hdc,RGB(0,255,0));
				
				hPen = CreatePen(PS_DOT , 1 , RGB(255,255,255));
				SelectObject(hdc,hPen);	
				
				MoveToEx(hdc,0,rect.top + 16,NULL);	
				LineTo(hdc,cxClient,rect.top+16);
				

				rect.top = rect.top + 16; 
				if(obj.szGender[0] != -1)
				{
					sprintf(stringDest,TEXT("Gender :"));	
					strcpy(stringSource,obj.szGender);
					strcat(stringDest,stringSource);
					DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT| DT_SINGLELINE);
				}
				memset(stringDest,0, 30*sizeof(TCHAR));
				
				MoveToEx(hdc,0,rect.top + 16,NULL);	
				LineTo(hdc,cxClient,rect.top+16);

				SetTextColor(hdc,RGB(0,0,255));
				for(iCounter = 0;iCounter<4;iCounter++)
				{	
					if(obj.bFavBatch[iCounter] == 1)
					{
						rect.top = rect.top + 16;
						if(iCounter == 0)
							strcpy(stringSource,TEXT("C"));
						else if(iCounter == 1)
							strcpy(stringSource,TEXT("C++"));
						else if(iCounter == 2)
							strcpy(stringSource,TEXT("UNIX"));
						else
							strcpy(stringSource,TEXT("ASSEMBLY"));

						strcat(stringDest,stringSource);
						DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT| DT_SINGLELINE);
						memset(stringDest,0, 30*sizeof(TCHAR));
					}
				}

				MoveToEx(hdc,0,rect.top + 16,NULL);	
				LineTo(hdc,cxClient,rect.top+16);
				
				rect.top = rect.top + 16;	
				SetTextColor(hdc,RGB(0,255,255));
				for(iCounter = 0;iCounter<10;iCounter++)
				{
					if(obj.arr[iCounter] == -1)
						break;

					SendMessage(hwndListBox,LB_GETTEXT,(WPARAM)obj.arr[iCounter],(LPARAM)stringSource);	
					strcat(stringDest,stringSource);
					DrawText(hdc,stringDest,-1,&rect,DT_TOP | DT_LEFT| DT_SINGLELINE);
					rect.top = rect.top + 16;	
					memset(stringDest,0, 30*sizeof(TCHAR));					
				}

				DeleteObject(hPen);
				EndPaint(hwnd,&ps);
				break;
			
			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}