#include<windows.h>
#include<strsafe.h>

#define BUFSIZE 65535
#define SHIFTED 0x8000

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

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
					TEXT("TOGGLE"),
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

	#if MAIN_LOG
		fclose(fp);
		fp = NULL;
	#endif //MAIN_LOG
	return (int)msg.wParam;
}

#define DIVISIONS 5

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		HDC hdc;
		int x,y;
		PAINTSTRUCT ps;
		HBRUSH hBrush;
		static int iCaretPosX,iCaretPosY;

		static int cxBlock,cyBlock;
		static BOOL fstate[DIVISIONS][DIVISIONS];
		static int cxClient; 		// Weidth of client area
		static int cyClient; 		// height of client area

		switch(uiMessage)
		{

			case WM_SIZE:

				cxBlock = LOWORD(lParam)/DIVISIONS;

				cyBlock = HIWORD(lParam)/DIVISIONS;
				
				
				// save the new width and height of the client area

				cxClient = LOWORD(lParam);
				cyClient = HIWORD(lParam);
		
				break;

			case WM_KEYDOWN:
				HideCaret(hwnd);

			
					switch(wParam)
					{
						case VK_LEFT:
							if(iCaretPosX > 0)
								iCaretPosX = iCaretPosX - cxBlock; 
							else
								MessageBeep(0);
							break;
						case VK_RIGHT:
							if(iCaretPosX + cxBlock*2 <= cxClient)
								iCaretPosX = iCaretPosX + cxBlock;
							else
								MessageBeep(0);
							break;

						case VK_UP:
							if(iCaretPosY == 0)
								MessageBeep(0);
							else
								iCaretPosY = iCaretPosY - cyBlock;

							break;

						case VK_DOWN:
							
							if(iCaretPosY + cyBlock*2 <= cyClient)
								iCaretPosY = iCaretPosY + cyBlock;
							else
								MessageBeep(0);
							break;	

						case VK_RETURN:
								x = iCaretPosX / cxBlock;
								y = iCaretPosY / cyBlock;

								if(x < DIVISIONS && y < DIVISIONS)
								{
									fstate[x][y] = fstate[x][y]^1;
									InvalidateRect(hwnd,NULL,FALSE);
								}	
								else
								{
									MessageBeep(0);
								}
							break;			

					}
					SetCaretPos(iCaretPosX , iCaretPosY);

				ShowCaret(hwnd);
				break;

		case WM_SETFOCUS:
			
			// create position and display the caret when the window receives the keyboard focus

			// CreateCaret(hwnd,(HBITMRP)1,0,cyChar);

			CreateCaret(hwnd,NULL,0,16);
			SetCaretPos(iCaretPosX , iCaretPosY);

			ShowCaret(hwnd);

			break;	

		case WM_KILLFOCUS:

			// hide and destroy the caret when the window loses the keyboard focus

			HideCaret(hwnd);
			DestroyCaret();

			break;

						
		case WM_LBUTTONDOWN:

				x = LOWORD(lParam)/cxBlock;
				y = HIWORD(lParam)/cyBlock;

				if(x < DIVISIONS && y < DIVISIONS)
				{
					fstate[x][y] = fstate[x][y]^1;
					InvalidateRect(hwnd,NULL,FALSE);
				}	
				else
				{
					MessageBeep(0);
				}
				
				break;	

			case WM_PAINT:
				
				hdc = BeginPaint(hwnd,&ps);
				HideCaret(hwnd);

				hBrush = CreateSolidBrush(RGB(9,184,176));
				SelectObject(hdc,hBrush);

				/*for(x = 0 ; x < cxClient ; x=x+cxBlock)
				{
				    MoveToEx(hdc,x,0,NULL);
					LineTo(hdc,x,cyClient);
				}
				for(y = 0; y < cyClient ; y=y+cyBlock)
				{
					MoveToEx(hdc,0,y,NULL);
					LineTo(hdc,cxClient,y);
				}*/

				for(x = 0; x < DIVISIONS ; x++)
				{
					for(y = 0; y < DIVISIONS ; y++)
					{
						Rectangle(hdc,x*cxBlock,y*cyBlock,(x+1)*cxBlock,(y+1)*cyBlock);

						if(fstate[x][y] == 1)
						{
							MoveToEx(hdc,x*cxBlock,y*cyBlock,NULL);
							LineTo(hdc,(x+1)*cxBlock,(y+1)*cyBlock);
							MoveToEx(hdc,(x+1)*cxBlock,y*cyBlock,NULL);
							LineTo(hdc,x*cxBlock,(y+1)*cyBlock);
						}
					}
				}
				DeleteObject(hBrush);
				ShowCaret(hwnd);
				EndPaint(hwnd,&ps);

				break;

			case WM_DESTROY:
		
				PostQuitMessage(0);
		
			break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);

}


