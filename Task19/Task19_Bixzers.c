#include<windows.h>
#include<stdio.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

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
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"),szAppName,MB_OK);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("PolyBezier"),
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
		MessageBox(hwnd,TEXT("CreateWindow : FAlLURE"),szAppName,MB_OK);
		return -1;	
	}

	ShowWindow(hwnd,iCmdShow);
	UpdateWindow(hwnd);

	while((bRet = GetMessage(&msg,NULL,0,0)) != 0)
	{
		if(bRet == -1)
		{
			MessageBox(NULL,TEXT("CreateWindow : FAlLURE"), szAppName , MB_OK);
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
	PAINTSTRUCT ps;

	static int cxClient;
	static int cyClient;

	static int x;
	static int y;
	RECT rect;

	POINT apt[4] = {0};
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			#if WND_PROC
				fputs("In WM_SIZE",fp);
			#endif	

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			break;
	case WM_PAINT:

			#if WND_PROC
				fputs("\nIn WM_PAINT\n",fp);
			#endif	

			hdc  = BeginPaint(hwnd,&ps);

			x = 0;
			y = cyClient / 2;

			apt[0].x = x; // Starting point
			apt[0].y = y;	

			x = cxClient / 8;
			y = cyClient / 8;

			apt[1].x = x; // second point 
			apt[1].y = y;	


			x = cxClient - (cxClient / 8);
			y = cyClient - (cyClient / 8);

			apt[2].x = x;  // Third point
			apt[2].y = y;

			x = cxClient;
			y = cyClient / 2;

			apt[3].x = x;   // end point 
			apt[3].y = y;

			Rectangle(hdc,apt[1].x , apt[1].y ,apt[2].x , apt[2].y);
			Chord(hdc,0,0,apt[1].x , apt[1].y ,0 ,0 ,0 ,0);
			Chord(hdc,apt[2].x , apt[2].y ,cxClient , cyClient,apt[2].x , apt[2].y,apt[2].x , apt[2].y);
			RoundRect(hdc,apt[1].x , apt[1].y ,apt[2].x , apt[2].y , (apt[2].y - apt[1].y)/4, (apt[2].x - apt[1].x)/4 );

			// For Beziers
			MoveToEx(hdc,apt[0].x,apt[0].y,NULL);
			PolyBezier(hdc,apt,MY_ARRAY_SIZE(apt));

			// For Cross Lines
			MoveToEx(hdc,apt[1].x , apt[1].y,NULL);
			LineTo(hdc, apt[2].x , apt[2].y);

			MoveToEx(hdc,apt[1].x , apt[2].y,NULL);
			LineTo(hdc, apt[2].x , apt[1].y);


			// For Print Text
			rect.top  = 0; 
			rect.left = 0;
			rect.right = apt[3].x;
			rect.bottom = apt[3].y;

			SetTextColor(hdc,RGB(120,5,28));
			SetBkColor(hdc,RGB(255,197,58));
			DrawText(hdc,TEXT("I am missing jyoti and taresh"),-1,&rect,DT_CENTER | DT_VCENTER | DT_SINGLELINE);


			EndPaint(hwnd,&ps);
			
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}