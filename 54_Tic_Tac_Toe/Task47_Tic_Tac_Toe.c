#include<windows.h>
#include<strsafe.h>

#define BUFSIZE 65535
#define SHIFTED 0x8000

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")
//#pragma comment(lib,"Winmm.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define GWL_HINSTANCE       (-6) // Winuser.h
#define DIVISIONS 3
int Check(BOOL [DIVISIONS][DIVISIONS],HWND);


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
					TEXT("TIC TAC TOE"),
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

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		HDC hdc;
		int x,y;
		PAINTSTRUCT ps;
		HBRUSH hBrush;
		HPEN hPen;
		static int iCaretPosX,iCaretPosY;

		static HWND hwndBtn[3] = {NULL};
		static int cxBlock,cyBlock,cxClient,cyClient;
		static BOOL bFlag = 0;
		static BOOL fstate[DIVISIONS][DIVISIONS];

		switch(uiMessage)
		{
			case WM_CREATE:

				hwndBtn[0] = CreateWindow(
					TEXT("BUTTON"),
					TEXT("RESET"),
					WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_PUSHBUTTON | BS_NOTIFY,
					100,
					100,
					100,
					100,
					hwnd,
					NULL,
					(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
					NULL
					);
				
				ShowWindow(hwndBtn[0],SW_HIDE);
				
			 	break;

		case WM_COMMAND:
				
				switch(HIWORD(wParam))
				{
					case BN_CLICKED:
							bFlag = 0;
							memset(fstate, 0, sizeof(fstate));
							InvalidateRect(hwnd,NULL,TRUE);
							break;
				}
				//PlaySound(TEXT("pacman_beginning.wav"),NULL,SND_ASYNC | SND_LOOP);
				
			
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
					if(iCaretPosX + cxBlock*2 <= (cxClient/2))
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
							
					if(iCaretPosY + cyBlock*2 <= (cyClient/2))
						iCaretPosY = iCaretPosY + cyBlock;
					else
						MessageBeep(0);
					break;	

				case VK_RETURN:	
					x = iCaretPosX/cxBlock;
					y = iCaretPosY/cyBlock;

					if(x < DIVISIONS && y < DIVISIONS)
					{
						if(bFlag == 0)
						{	
							if(fstate[x][y] == 2)
							{

							}
							else{
								fstate[x][y] = 1;
								bFlag = 2;
								}
						}
						else
						{
							if(fstate[x][y] == 1)
							{
							}
							else{
								fstate[x][y] = 2;
								bFlag = 0;
								}
										
							}
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

	
		case WM_SIZE:

			//	PlaySound(TEXT("pacman_beginning.wav"),NULL,SND_ASYNC | SND_LOOP);
				cxClient  = LOWORD(lParam);
				cyClient  = HIWORD(lParam);

				cxBlock = (LOWORD(lParam)/2)/DIVISIONS;
				cyBlock = (HIWORD(lParam)/2)/DIVISIONS;

				MoveWindow(hwndBtn[0],1*cxBlock,(3+1)*cyBlock,0.5*cxBlock,0.5*cyBlock,TRUE);
				ShowWindow(hwndBtn[0],SW_SHOWNORMAL);
				break;

		case WM_LBUTTONDOWN:

				x = (LOWORD(lParam))/cxBlock;
				y = (HIWORD(lParam))/cyBlock;

				if(x < DIVISIONS && y < DIVISIONS)
				{
					if(bFlag == 0)
					{	
						if(fstate[x][y] == 2)
						{

						}
						else{
						fstate[x][y] = 1;
						bFlag = 2;
						}
					}
					else
					{
						if(fstate[x][y] == 1)
						{

						}
						else{
							fstate[x][y] = 2;
							bFlag = 0;
						}
						
					}
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
				for(x = 0; x < DIVISIONS ; x++)
				{
					for(y = 0; y < DIVISIONS ; y++)
					{
						SelectObject(hdc,GetStockObject(WHITE_BRUSH));
						RoundRect(hdc,x*cxBlock,y*cyBlock,(x+1)*cxBlock,(y+1)*cyBlock,50,50);
						//Rectangle(hdc,x*cxBlock,y*cyBlock,(x+1)*cxBlock,(y+1)*cyBlock);

						if(fstate[x][y] == 1)
						{
							MoveToEx(hdc,(x+0.2)*cxBlock,(y+0.2)*cyBlock,NULL);
							LineTo(hdc,(x-0.2+1)*cxBlock,(y-0.2+1)*cyBlock);
							MoveToEx(hdc,(x-0.2+1)*cxBlock,(y+0.2)*cyBlock,NULL);
							LineTo(hdc,(x+0.2)*cxBlock,(y-0.2+1)*cyBlock);
						}
						else if(fstate[x][y] == 2)
						{
							hBrush = CreateSolidBrush(RGB(0,255,0));
							SelectObject(hdc,hBrush);

							RoundRect(hdc,(x+0.2)*cxBlock,(y+0.2)*cyBlock,(x-0.2+1)*cxBlock,(y-0.2+1)*cyBlock,50,50);
							DeleteObject(hBrush);
						}
					}
				}

				hPen = CreatePen(PS_DASHDOT , 1 , RGB(221,30,2));
				SelectObject(hdc,hPen);
				
	
				MoveToEx(hdc,(cxClient-(3*cxBlock))+((cxClient-(3*cxBlock))/2),0,NULL);
				LineTo(hdc,(cxClient-(3*cxBlock))+((cxClient-(3*cxBlock))/2),cyClient);
				
				DeleteObject(hPen);
				
				if(Check(fstate,hwnd)== TRUE)
				{
					//PlaySound(TEXT("pacman_intermission.wav"),NULL,SND_ASYNC | SND_LOOP);
					memset(fstate,0,sizeof(fstate));
				}
				ShowCaret(hwnd);
				EndPaint(hwnd,&ps);
				break;
			case WM_DESTROY:
				
				PostQuitMessage(0);
		
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

int Check(BOOL fstate[DIVISIONS][DIVISIONS],HWND hwnd)
{
				int x,y;
				static int iCrossCount = 0;
				static int iCircleCount = 0;
				// Left to right check
				for(x = 0; x < DIVISIONS ; x++)
				{
					for(y = 0; y < DIVISIONS ; y++)
					{
						if(fstate[y][x] == 1)
						{
							iCrossCount++;
							if(iCrossCount == 3)
							{
								MessageBox(hwnd,TEXT("CROSS WON"),TEXT("Left to right"),MB_OK);
								return TRUE;	
							}
						}
						if(fstate[y][x] == 2)
						{
							iCircleCount++;
							if(iCircleCount == 3)
							{
								MessageBox(hwnd,TEXT("CIRCLE WON"),TEXT("Left to right"),MB_OK);
								return TRUE;

							}
						}
					}
					iCrossCount = 0;
					iCircleCount = 0;
				}

				// top to bottom check
				for(x = 0; x < DIVISIONS ; x++)
				{
					for(y = 0; y < DIVISIONS ; y++)
					{
						if(fstate[x][y] == 1)
						{
							iCrossCount++;
							if(iCrossCount == 3)
							{
								MessageBox(hwnd,TEXT("CROSS WON"),TEXT("top to bottom"),MB_OK);
								return TRUE;
							}
						}
						if(fstate[x][y] == 2)
						{
							iCircleCount++;
							if(iCircleCount == 3)
							{
								MessageBox(hwnd,TEXT("CIRCLE WON"),TEXT("top to bottom"),MB_OK);
								return TRUE;
								
							}
						}
					}
					iCrossCount = 0;
					iCircleCount = 0;
				}

				// cross check
				
				if(((fstate[0][0] == 1  && fstate[2][2] == 1) || (fstate[0][2] == 1 && fstate[2][0] == 1)) && fstate[1][1] == 1)
				{
					MessageBox(hwnd,TEXT("CROSS WON"),TEXT("cross check"),MB_OK);
					return TRUE;
				}
				if(((fstate[0][0] == 2 && fstate[2][2] == 2) || (fstate[0][2] == 2 && fstate[2][0] == 2))  && fstate[1][1] == 2)
				{
					MessageBox(hwnd,TEXT("CIRCLE WON"),TEXT("cross check"),MB_OK);
					return TRUE;
				}

				return FALSE;
}