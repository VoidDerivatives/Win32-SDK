#include<windows.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

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
					TEXT("CLOCK"),
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
		RECT rect;
		HBRUSH hBrush;	
		HPEN hPen,hPen1;
		static int cxClient;
		static int cyClient;
		TCHAR szText[255];
		TCHAR TempszText[255];
		SYSTEMTIME systemtime;
		HFONT hFont1;
		static POINT pt1[5] = {330,200 , 412,200 , 412,229 , 412,255};
		
		static POINT pt2[7] = {430,234 , 430,245, 431,245 , 457,245 , 457,234,430,234};
		POINT pt3;
		int iCounter;

		static POINT ptPrevious;
		switch(uiMessage)
		{	

			case WM_CREATE:
				SetTimer(hwnd,TimerId1,5000,(TIMERPROC)TimerProc1);
				break;

			case WM_SIZE:

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
			
				break;
				
			case WM_PAINT:


				hdc = BeginPaint(hwnd,&ps);


				hPen = CreatePen(PS_SOLID , 1 , RGB(255,255,255));
				SelectObject(hdc,hPen);

				// Background Color
				hBrush = CreateSolidBrush(RGB(54,62,176));
				SelectObject(hdc,hBrush);

				GetClientRect(hwnd,&rect);

				FillRect(hdc , &rect , hBrush);
				DeleteObject(hBrush);
				// Background Color End

				hBrush = CreateSolidBrush(RGB(0,0,0));
				SelectObject(hdc,hBrush);
				rect.top = (cyClient / 3);   //y
 				rect.left = (cxClient / 4);  //x
				rect.bottom = cyClient - (cyClient / 3); //y
				rect.right = cxClient - (cxClient / 4);    //x
				Rectangle(hdc,rect.left,rect.top,rect.right,rect.bottom);
				DeleteObject(hBrush);

				hPen1 = CreatePen(PS_SOLID , 1 , RGB(255,0,0));
				SelectObject(hdc,hPen1);
				Rectangle(hdc,rect.left+3,rect.top+3,rect.right-3,rect.bottom-3);
				DeleteObject(hPen1);

				hBrush = CreateSolidBrush(RGB(54,62,176));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,rect.left + 30,rect.top + 30,rect.right - 30,rect.bottom - 30);
				DeleteObject(hBrush);
				rect.top = (cyClient / 3) - 65;   //y
 				rect.left = (cxClient / 3);  //x
				rect.bottom = cyClient - (cyClient / 2); //y
				rect.right = cxClient - (cxClient / 3);    //x

				hBrush = CreateSolidBrush(RGB(0,0,0));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,rect.left + 60,rect.top + 30,rect.right - 60,rect.bottom - 30);
				
				hPen1 = CreatePen(PS_SOLID , 1 , RGB(255,0,0));
				SelectObject(hdc,hPen1);
				Rectangle(hdc,rect.left + 63,rect.top + 33,rect.right - 63,rect.bottom - 33);
				DeleteObject(hBrush);
				DeleteObject(hPen1);


				//////////////////////////////////////////////////////////
				rect.top = rect.top + 40;   //y
 				rect.left = rect.left + 68;  //x
				rect.bottom = rect.bottom - 27; //y
				rect.right = rect.right - 65;    //x

				GetLocalTime(&systemtime);

				SetTextColor(hdc,RGB(255,255,255));
				SetBkMode(hdc,TRANSPARENT);

				if(systemtime.wDayOfWeek == 0)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("SUN"));
				else if(systemtime.wDayOfWeek == 1)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("MON"));
				else if(systemtime.wDayOfWeek == 2)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("TUE"));
				else if(systemtime.wDayOfWeek == 3)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("WED"));
				else if(systemtime.wDayOfWeek == 4)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("THU"));
				else if(systemtime.wDayOfWeek == 5)
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("FRI"));
				else 
					_stprintf_s(TempszText , sizeof(TempszText) , TEXT("SAT"));

				hFont1 = CreateFont(35,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont1);
				_stprintf_s(szText , sizeof(szText) , TEXT("%d:%d"),systemtime.wHour,systemtime.wMinute);
				DrawText(hdc,szText,-1,&rect,DT_LEFT | DT_TOP );
				DeleteObject(hFont1);


				hFont1 = CreateFont(13,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont1);
				_stprintf_s(szText , sizeof(szText) , TEXT("\n\n\nHeart Rate:%d\nWalk :%d\ncalories:%d"),92,1000,400);
				DrawText(hdc,szText,-1,&rect,DT_LEFT | DT_TOP );
				DeleteObject(hFont1);


				hFont1 = CreateFont(35,0,0,0,FW_REGULAR,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont1);
				_stprintf_s(szText , sizeof(szText) , TEXT("%d/%d"),systemtime.wDay,systemtime.wMonth);
				DrawText(hdc,szText,-1,&rect,DT_RIGHT | DT_TOP );
				DeleteObject(hFont1);
				
				hFont1 = CreateFont(15,0,0,0,FW_REGULAR,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont1);
				_stprintf_s(szText , sizeof(szText) , TEXT("\n\n%s"),TempszText);
				DrawText(hdc,szText,-1,&rect, DT_RIGHT | DT_TOP);
				

				_stprintf_s(szText , sizeof(szText) , TEXT("\n\n\n%d"),systemtime.wYear);
				DrawText(hdc,szText,-1,&rect,DT_RIGHT | DT_TOP );
				DeleteObject(hFont1);



				rect.top = 234;   //y
 				rect.left = 412;  //x
				rect.bottom = 245;
				rect.right = 430;

				hFont1 = CreateFont(11,0,0,0,FW_BOLD,FALSE,FALSE,FALSE,DEFAULT_CHARSET,OUT_OUTLINE_PRECIS,
	                CLIP_DEFAULT_PRECIS,CLEARTYPE_QUALITY, VARIABLE_PITCH,TEXT(""));
	            SelectObject(hdc, hFont1);
	            _stprintf_s(szText , sizeof(szText) , TEXT("%d%%"),55);
				DrawText(hdc,szText,-1,&rect,DT_RIGHT | DT_BOTTOM | DT_SINGLELINE );
				DeleteObject(hFont1);




				MoveToEx(hdc,pt1[0].x,pt1[0].y,NULL);
				for(iCounter=0;iCounter<MY_ARRAY_SIZE(pt1)-1;iCounter++)
					LineTo(hdc,pt1[iCounter].x,pt1[iCounter].y);

				MoveToEx(hdc,pt2[0].x,pt2[0].y,NULL);
				for(iCounter=0;iCounter<MY_ARRAY_SIZE(pt2)-1;iCounter++)
					LineTo(hdc,pt2[iCounter].x,pt2[iCounter].y);

				DeleteObject(hPen);

				hPen1 = CreatePen(PS_SOLID , 1 , RGB(255,0,0));
				SelectObject(hdc,hPen1);
				hBrush = CreateSolidBrush(RGB(255,255,255));
				SelectObject(hdc,hBrush);
				Rectangle(hdc,444,234,457,245);
				DeleteObject(hBrush);
				DeleteObject(hPen);

				EndPaint(hwnd,&ps);
				break;
	
			case WM_DESTROY:
				
				PostQuitMessage(0);
				KillTimer(hwnd,TimerId1);
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
void CALLBACK TimerProc1(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	InvalidateRect(hwnd,NULL,TRUE);
}