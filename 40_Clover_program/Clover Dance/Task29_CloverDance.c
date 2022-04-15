#include<windows.h>
#include<math.h>
#include<tchar.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT   600
#define TWO_PIE 2.0*3.14156


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
					TEXT("COMBO DIFFERENT STYLE AND TRANSPARENT MODE"),
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

			MessageBox(NULL,TEXT("CreateWindow : FAlL;/?URE"), szAppName , 0);
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
	HDC hdc;
	RECT rect;
	PAINTSTRUCT ps;
	HRGN hRgnTemp[6];
	static HRGN hRgnCliped; 
	double dRadius,dAngle;
	static int cxClient;
	static int cyClient;
	static int iColor;
	int iCounter;
	HPEN hPen;
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			if(hRgnCliped)
					DeleteObject(hRgnCliped);

			hRgnTemp[0] = (HRGN)CreateEllipticRgn(0,cyClient/3,cxClient/2,2*cyClient/3);
			
			hRgnTemp[1] = (HRGN)CreateEllipticRgn(cxClient/2,cyClient/3,cxClient,2*cyClient/3);

			hRgnTemp[2] = (HRGN)CreateEllipticRgn(cxClient/3,0,2*cxClient/3,cyClient/2);

			hRgnTemp[3] = (HRGN)CreateEllipticRgn(cxClient/3,cyClient/2,2*cxClient/3,cyClient);

			hRgnTemp[4] = CreateRectRgn(0,0,1,1);

			hRgnTemp[5] = CreateRectRgn(0,0,1,1);

			hRgnCliped = CreateRectRgn(0,0,1,1);

			CombineRgn(hRgnTemp[4],hRgnTemp[0],hRgnTemp[1],RGN_OR);

			CombineRgn(hRgnTemp[5],hRgnTemp[2],hRgnTemp[3],RGN_OR);

			CombineRgn(hRgnCliped,hRgnTemp[4],hRgnTemp[5],RGN_XOR);

			for(iCounter = 0;iCounter<6;iCounter++)
			{
				DeleteObject(hRgnTemp[iCounter]);
			}

			break;

	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);
			hPen = CreatePen(PS_SOLID , 1 , RGB(163,73,164));
			if(iColor == 0)
				hPen = CreatePen(PS_SOLID , 1 , RGB(163,73,164));
			else if(iColor == 1)
				hPen = CreatePen(PS_SOLID , 1 , RGB(255,0,0));
			else if(iColor == 2)
				hPen = CreatePen(PS_SOLID , 1 , RGB(0,255,0));
			else 
				hPen = CreatePen(PS_SOLID , 1 , RGB(0,0,255));

			SelectObject(hdc,hPen);

			SetViewportOrgEx(hdc,cxClient/2,cyClient/2,NULL);

			SelectClipRgn(hdc,hRgnCliped);

			dRadius = _hypot(cxClient/2.0,cyClient/2.0);
			


			for(dAngle = 0.0; dAngle < TWO_PIE;dAngle = dAngle + TWO_PIE/360)
			{

				MoveToEx(hdc,0,0,NULL);
				LineTo(hdc,(int)(dRadius*cos(dAngle)+0.5),(int)(-dRadius * sin(dAngle)+0.5));

			}
	

			iColor++;
			if(iColor == 4)
				iColor = 0;
			EndPaint(hwnd,&ps);
			DeleteObject(hPen);
			
			break;
	
	case WM_DESTROY:
			DeleteObject(hRgnCliped);
			PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}