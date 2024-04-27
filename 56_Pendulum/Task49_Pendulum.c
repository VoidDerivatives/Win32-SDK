#include<windows.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")
#pragma comment(lib,"Winmm.lib")

#define TimerId1		1


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void CALLBACK TimerProc1(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

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
					TEXT("Pendulum"),
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

int cxClient;
int cyClient;
int iTime;

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		HDC hdc;
		RECT rect;
		HPEN hPen;		
		static int cxClient;
		static int cyClient;
		HBRUSH hBrush;
		static BOOL bFlag1 = TRUE;
		static BOOL bFlag2 = FALSE;
		static BOOL bFlag3 = FALSE;
		static BOOL bBackUP = FALSE;
		TCHAR szText[255];

		switch(uiMessage)
		{	

			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
			
				bFlag1 = TRUE;
				bFlag2 = FALSE;
				bFlag3 = FALSE;
				break;
			case WM_KEYDOWN:
				switch(wParam)
				{
					case VK_RETURN:
						iTime = 300;
						SetTimer(hwnd,TimerId1,iTime,(TIMERPROC)TimerProc1);
						PlaySound(TEXT("pendulum.wav"),NULL,SND_ASYNC | SND_LOOP);
					break;
				}
				break;

			case WM_LBUTTONDOWN:
				iTime = 300;
				SetTimer(hwnd,TimerId1,iTime,(TIMERPROC)TimerProc1);
				PlaySound(TEXT("pendulum.wav"),NULL,SND_ASYNC | SND_LOOP);
				break;

			case WM_PAINT:
				
				hdc = BeginPaint(hwnd,&ps);

				hPen = CreatePen(PS_SOLID , 1 , RGB(163,73,164));
				SelectObject(hdc,hPen);

				rect.top = (cyClient / 3);   //y
 				rect.left = (cxClient / 4);  //x
				rect.bottom = (cyClient / 2) - (cyClient / 5); //y
				rect.right = (cxClient / 2) + cxClient / 4;    //x
				
				Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);

				hBrush = CreateSolidBrush(RGB(163,73,164));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,rect.left + 3,rect.top - 3,rect.right - 3,rect.bottom + 3);
				DeleteObject(hBrush);
	
				if(bFlag1)
				{
					MoveToEx(hdc,cxClient/2,(cyClient / 2) - (cyClient / 5),NULL);
					LineTo(hdc,cxClient/2,(cyClient / 2));
					
					rect.top = cyClient / 2;   //y
					rect.left = cxClient/2 - 40;   //x
					rect.bottom = cyClient / 2 + 90; //y
					rect.right = cxClient/2 + 50;    //x

					hBrush = CreateSolidBrush(RGB(0,0,0));
					SelectObject(hdc,hBrush);
					RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,80,80);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush(RGB(163,73,164));
					SelectObject(hdc,hBrush);
					RoundRect(hdc,(rect.left) + 10,(rect.top) + 10 ,(rect.right) - 10,(rect.bottom) - 10,80,80);
					DeleteObject(hBrush);
					
					bFlag1 = FALSE;
					if(bBackUP == FALSE)
					{
						bFlag2 = TRUE;
						bBackUP = TRUE;
					}
					else
					{
						bFlag3 = TRUE;
						bBackUP = FALSE;	
					}
				}
				//************************************************************
				else if(bFlag2)
				{
					
						MoveToEx(hdc,cxClient/2,(cyClient / 2) - (cyClient / 5),NULL);
						LineTo(hdc,cxClient/2 - 120,(cyClient / 2));
						

						rect.top = cyClient / 2;
						rect.left = cxClient/2 - 180;
						rect.bottom = cyClient / 2 + 90; //y
						rect.right = cxClient/2 - 90;    //x


						hBrush = CreateSolidBrush(RGB(0,0,0));
						SelectObject(hdc,hBrush);
						RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,80,80);
						DeleteObject(hBrush);

						hBrush = CreateSolidBrush(RGB(163,73,164));
						SelectObject(hdc,hBrush);
						RoundRect(hdc,(rect.left) + 10,(rect.top) + 10 ,(rect.right) - 10,(rect.bottom) - 10,80,80);
						DeleteObject(hBrush);
						bFlag2 = FALSE;
						bFlag1 = TRUE;
					
				}
				//************************************************************
				else if(bFlag3)
				{
					MoveToEx(hdc,cxClient/2,(cyClient / 2) - (cyClient / 5),NULL);
					LineTo(hdc,cxClient/2 + 120,(cyClient / 2));
					
					rect.top = cyClient / 2;    //y
					rect.left = cxClient/2 + 180;    //x
					rect.bottom = cyClient / 2 + 90; //y
					rect.right = cxClient/2 + 90;    //x

					hBrush = CreateSolidBrush(RGB(0,0,0));
					SelectObject(hdc,hBrush);
					RoundRect(hdc,rect.left,rect.top,rect.right,rect.bottom,80,80);
					DeleteObject(hBrush);

					hBrush = CreateSolidBrush(RGB(163,73,164));
					SelectObject(hdc,hBrush);
					RoundRect(hdc,(rect.left) - 10,(rect.top) + 10 ,(rect.right) + 10,(rect.bottom) - 10,80,80);
					DeleteObject(hBrush);
					bFlag3 = FALSE;
					bFlag1 = TRUE;
				}
				//************************************************************

				if(iTime >= 1500)
				{
					KillTimer(hwnd,TimerId1);
					iTime = 0;
					PlaySound(NULL, NULL, 0); 
				}
	
				SetBkColor(hdc,RGB(128,128,255));
				rect.top = 0;   //y
	 			rect.left = 0;  //x
				rect.bottom = (cyClient / 2) - (cyClient / 5); //y
				rect.right = cxClient;    //x
				if(iTime == 0)
				{
					DrawText(hdc,TEXT("Not Moving"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}
				else
				{
					_stprintf_s(szText , sizeof(szText) , TEXT("Speed Of Pendulum is %d Second"),iTime/100);
					DrawText(hdc,szText,-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
				}	

				DeleteObject(hPen);
				EndPaint(hwnd,&ps);
				break;
			case WM_DESTROY:
				
				PostQuitMessage(0);
				//KillTimer(hwnd,TimerId1);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
void CALLBACK TimerProc1(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	static int iCheck;
	static int iTimeTemp = 100;
	static int iCounter = 0;
	if(iCheck == 6)
	{
		iTime = iTime + iTimeTemp + (iCounter * 10);
		SetTimer(hwnd,TimerId1,iTime,(TIMERPROC)TimerProc1);
		iCheck = -1;
	}
	iCheck++;
	InvalidateRect(hwnd,NULL,TRUE);
}
