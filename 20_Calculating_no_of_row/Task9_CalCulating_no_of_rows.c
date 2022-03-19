#include<windows.h>
#include<stdio.h>
#include<tchar.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT   600

int WINAPI WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrecInstance,
					LPSTR     pszCmdLine,
					int      iCmdShow
					)
{
	MSG msg;
	WNDCLASSEX wndclass;
	BOOL bRet;
	HWND hwnd;

	int cxScreen;
	int cyScreen;

	int x;
	int y;

	TCHAR szAppName[] = TEXT("CenterWindow");

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
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"),szAppName,0);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("Counting"),
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
			MessageBox(NULL,TEXT("CreateWindow : FAlLURE"), szAppName , 0);
			return -1;
		}
		TranslateMessage(&msg);

		DispatchMessage(&msg);
	}
	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc;
	TEXTMETRIC tm;
	TCHAR szText[255];

	int iWidth;

	static int cyClient;
	static int iNoOfRows;
	static int cyChar;
	//static int cxChar;
	//static int cxCaps;
	int iCounter = 0;

	switch(uiMessage)
	{			

	case WM_CREATE:
			hdc = GetDC(hwnd);

			GetTextMetrics(hdc,&tm);

			cyChar = tm.tmHeight;

			/*if((tm.tmPitchAndFamily & 1 ) != 0)
				cxCaps = 3 * cxChar / 2 ;
			else
				cxCaps = cxChar;
			*/
			ReleaseDC(hwnd,hdc);	

			break;	

	case WM_SIZE:

			GetClientRect(hwnd,&rect);

			//iHeight = 0;

			//cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam); 
			iNoOfRows = cyClient / cyChar;


			//rect.left = 0;
			//rect.top = 0;
			//rect.right = cxClient;
			//rect.bottom = cyClient;
			// v1
			/*while(iHeight < rect.bottom)
			{
				iHeight = iHeight + cyChar;
				iNoOfRows++;
			}*/

			// v2 iNoOfRows = rect.bottom / chChar;	

			break;		
	
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			iWidth = 0 ;
			iCounter = 1;
			while(iCounter <= iNoOfRows)
			{
				_stprintf_s(szText,sizeof(szText),TEXT("%d"),iCounter);
				TextOut(hdc , 0 , iWidth , szText,(int)_tcslen(szText));
				iWidth = iWidth + cyChar;
				iCounter++;	
			}

			GetClientRect(hwnd,&rect);

			SetTextColor(hdc,RGB(255,0,0));
			SetBkColor(hdc,RGB(0,255,0));
			_stprintf_s(szText , sizeof(szText) , TEXT("Total No of Row Count Fit in Window is %d"),iNoOfRows);

			DrawText(hdc , szText , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			ReleaseDC(hwnd,hdc);
			break;

	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}