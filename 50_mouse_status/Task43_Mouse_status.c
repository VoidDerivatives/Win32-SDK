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
		MessageBox(NULL,TEXT("RegisterClassEx : FalLURE"),szAppName,0);
		return -1;
	} 

	cxScreen = GetSystemMetrics(SM_CXSCREEN);
	cyScreen = GetSystemMetrics(SM_CYSCREEN);

	x = ( cxScreen / 2 ) - (WINDOW_WIDTH / 2);
	y = ( cyScreen / 2 ) - (WINDOW_HEIGHT / 2);
	hwnd = CreateWindow(
					szAppName,
					TEXT("MOUSE STATUS"),
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
	HDC hdc;
	int iMouse;
	RECT rect;
	static int cxClient;
	static int cyClient;
	char Buffer[1000];
	HPEN hPen;
	
	switch(uiMessage)
	{		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);
			break;
			
	case WM_PAINT:
			hdc  = BeginPaint(hwnd,&ps);

			SetTextColor(hdc,RGB(255,255,255));
			SetBkMode(hdc,TRANSPARENT);
			rect.top =  0;
			rect.left = 0;
			rect.bottom = cyClient / 5;
			rect.right = cxClient;

			iMouse = GetSystemMetrics(SM_MOUSEPRESENT);
			if(iMouse == TRUE)
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEPRESENT  => MOUSE is Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			else
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEPRESENT => MOUSE is Not Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);


			rect.top = cyClient / 5;
			rect.left = 0;
			rect.bottom = cyClient / 3;
			rect.right = cxClient;
			iMouse = GetSystemMetrics(SM_MOUSEWHEELPRESENT);

			if(iMouse == TRUE)
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEWHEELPRESENT => MOUSE WHEEL is Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			else
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEWHEELPRESENT => MOUSE WHEEL is Not Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		

			rect.top = cyClient / 3;
			rect.left = 0;
			rect.bottom = cyClient / 2;
			rect.right = cxClient;

			iMouse = GetSystemMetrics(SM_MOUSEHORIZONTALWHEELPRESENT);

			if(iMouse == TRUE)
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEHORIZONTALWHEELPRESENT => MOUSE HORIZONTAL WHEEL is Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			else
				DrawText(hdc , TEXT("[Message] :- SM_MOUSEHORIZONTALWHEELPRESENT => MOUSE HORIZONTAL WHEEL is Not Present") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);


			rect.top = cyClient / 2;
			rect.left = 0;
			rect.bottom = cyClient - (cyClient/3);
			rect.right = cxClient;

			iMouse = GetSystemMetrics(SM_CMOUSEBUTTONS);

			sprintf(Buffer,"[Message] :- SM_CMOUSEBUTTONS => MOUSE Count is : %d",iMouse);
			if(iMouse != 0)
				DrawText(hdc , Buffer , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			else
				DrawText(hdc , TEXT("[Message] :- SM_CMOUSEBUTTONS => is Not Installed") , -1 , &rect , DT_CENTER | DT_VCENTER | DT_SINGLELINE);



			EndPaint(hwnd,&ps);
			break;
	
	case WM_DESTROY:
				PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}