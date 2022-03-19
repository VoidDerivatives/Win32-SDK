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
					TEXT("FORTS NAME"),
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

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
	PAINTSTRUCT ps;
	RECT rect;
	HDC hdc;
	TEXTMETRIC tm;
	int iCounter;
	int iWidth;
	static int cxChar;
	static int cxCaps;
	static int iNoOfRows;
	static int cxClient;

	TCHAR *szFort[] = {TEXT("Red"),
						TEXT("Mehrangarh"),
						TEXT("Gwalior"),
						TEXT("Agra"),
						TEXT("Kangra"),
						TEXT("Jhansi"),
						TEXT("Rajgad"),
						TEXT("Chittorgarh")};
	
	switch(uiMessage)
	{			

	case WM_CREATE:
			hdc = GetDC(hwnd);

			GetTextMetrics(hdc,&tm);

			cxChar = tm.tmAveCharWidth;
			//cyChar = tm.tmHeight;

			if((tm.tmPitchAndFamily & 1 ) != 0)
				cxCaps = 3 * cxChar / 2 ;
			else
				cxCaps = cxChar;
			
			ReleaseDC(hwnd,hdc);	

			break;	

	case WM_SIZE:

			cxClient = LOWORD(lParam); 
			iNoOfRows = cxClient / cxCaps;
		
			break;		
	
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			SetTextColor(hdc,RGB(255,0,0));
			SetBkColor(hdc,RGB(255,255,255));
		
			iCounter = 0;
			iWidth = 0;
			while((iCounter < MY_ARRAY_SIZE(szFort)) && iWidth < cxClient)
			{
				TextOut(hdc , iWidth , 0 , szFort[iCounter],(int)_tcslen(szFort[iCounter]));
				iWidth = iWidth + cxChar + (cxCaps*(int)_tcslen(szFort[iCounter]));
				iCounter++;	
			}

			SetTextColor(hdc,RGB(0,0,255));
			SetBkColor(hdc,RGB(255,255,128));
			
			GetClientRect(hwnd,&rect);
			DrawText(hdc,"Void Derivatives Presents",-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			EndPaint(hwnd,&ps);
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}