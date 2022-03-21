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
					TEXT("THREE SHAPES"),
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
	HDC hdc;
	int iX2;
	int iY2;
	int iY3;
	int iX1;
	int iY1;
	RECT rect;
	//HBRUSH brush;
	static int cxClient;
	static int cyClient;
	POINT apt[4];
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
		
			break;
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			SelectObject(hdc, (HBRUSH) GetStockObject(BLACK_BRUSH));

			SetBkColor(hdc,RGB(128,128,255));

			iX2 = (cxClient / 2) - 10;
			iY2 = (cyClient / 2) - 10;
			iX1 = (cxClient / 12);
			iY1 = (cyClient / 12);

			rect.top = iY1;
			rect.left = iX1;
			rect.bottom = iY2;
			rect.right = iX2;

			Rectangle(hdc,iX1,iY1,iX2,iY2);
			DrawText(hdc,TEXT("Rectangle"),-1,&rect,DT_CENTER|DT_VCENTER | DT_SINGLELINE);

			iX2 = (cxClient / 2) + 10;
			iX1 = cxClient - (cxClient / 12);

			rect.left = iX1;
			rect.right = iX2;
			
			Ellipse(hdc,iX2,iY1,iX1,iY2);
			DrawText(hdc,TEXT("Ellipse"),-1,&rect,DT_CENTER|DT_VCENTER | DT_SINGLELINE);

			iX1 = cxClient / 2;
			iY1 = (cyClient / 2);
			
			iY3 = cyClient - 20;

			iX2 = (cxClient / 4);
			iY2 =  cxClient - (cxClient / 4); 

			rect.top = iY1;
			rect.left = iX2;
			rect.bottom = iY3;
			rect.right = iY2;

			apt[0].x = iX1;
			apt[0].y = iY1;
			MoveToEx(hdc,iX1,iY1,NULL);

			apt[1].x = iX2;
			apt[1].y = iY3;

			apt[2].x = iY2;
			apt[2].y = iY3;
			apt[3].x = iX1;
			apt[3].y = iY1;
			PolylineTo(hdc,apt+1, MY_ARRAY_SIZE(apt)-1);
			DrawText(hdc,TEXT("Triangle"),-1,&rect,DT_CENTER|DT_VCENTER | DT_SINGLELINE);

			EndPaint(hwnd,&ps);
			DeleteObject(SelectObject(hdc, (HBRUSH) GetStockObject(WHITE_BRUSH)));
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}