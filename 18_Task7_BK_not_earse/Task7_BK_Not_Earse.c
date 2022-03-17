#include<windows.h>

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

	TCHAR szAppName[] = TEXT("InWindowParty");

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
					TEXT("BACKGROUND NOT EARSE"),
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

	switch(uiMessage)
	{			
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			SetTextColor(hdc,RGB(17,2,204));
			SetBkColor(hdc,RGB(55,155,155));
			
			TextOut(hdc,0,0,TEXT("Please Click Left Button of Mouse FOR See the Magic"),
						sizeof(TEXT("Please Click Left Button of Mouse FOR See the Magic")));
		
			EndPaint(hwnd,&ps);
			break;

	case WM_LBUTTONDOWN:
				hdc = GetDC(hwnd);

				GetClientRect(hwnd,&rect);
				SetTextColor(hdc,RGB(13,12,0));
				SetBkColor(hdc,RGB(113,100,98));
				DrawText(hdc , TEXT("There is No Magic , APRIL FOOL !!!!!"),-1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
	
				ReleaseDC(hwnd,hdc);
				break;
	case WM_DESTROY:
				PostQuitMessage(0);
				break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
