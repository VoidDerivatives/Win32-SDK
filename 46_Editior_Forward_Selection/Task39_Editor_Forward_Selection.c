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
					TEXT("FOCUS YES OR NO"),
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

	return (int)msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
	HDC hdc;     				// Handle to device context
	TEXTMETRIC tm; 				// structure for text metrics
	static int cxChar;			// average width of characters
	static int cyChar;			// Height of Characters
	static int cxClient; 		// Weidth of client area
	static int cyClient; 		// height of client area

	static int iLines;			// text lines in client area
	static int iLineLen; 		// line length

	static int iCaretPosX;		// Horizontal position of caret
	static int iCaretPosY; 		// Vertical position of caret
	static int iCharWidth; 		// width of a character

	static int cch;				// character in buffer
	static int iCurChar;		// index of current character 

	TCHAR ch;					// Character character
	size_t uiLength; 			
	TCHAR szBuf[128];			// temporary buffer

	RECT rc;					// output rectangle for DrawText
	SIZE sz;					// string dimensions 
	PAINTSTRUCT ps;				// requried by BeginPaint
	COLORREF crPrevBk;			// previous background color
	COLORREF crPrevText;		// previous text color

	int i,j;					// loop counters
	int iVirtKey;				// Virtual - key code
	HRESULT hResult; 

	int iCR = 0;				// count of carriage returns
	int iCRIndex = 0;			// index of last carriage return

	static TCHAR *pchInputBuf;		// input Buffer

	static int iCaretPosXTemp;
	static int iCurCharTemp;
	static int iCaretPosYTemp;
	POINT point;

	int iVKReturnTemp;
	TCHAR chTemp;	
	
	
	switch(uiMessage)
	{		
	case WM_CREATE:
			// Get the metrics of the current font

			hdc = GetDC(hwnd);
			GetTextMetrics(hdc,&tm);
			ReleaseDC(hwnd,hdc);	
			
			// save the Average character width and height

			cxChar = tm.tmAveCharWidth;
			cyChar = tm.tmHeight + tm.tmExternalLeading;

			// Allocate a buffer to store keyboard input.

			pchInputBuf = (LPSTR) GlobalAlloc(GMEM_FIXED | GMEM_ZEROINIT /* GPTR */ , BUFSIZE * sizeof(TCHAR));

			break;

	case WM_SIZE:

			// save the new width and height of the client area

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			// calculate the maximum width of a line and the maximum number of lines in the client area

			iLineLen = cxClient - cxChar;
			iLines = cyClient / cyChar;

			break;

	case WM_SETFOCUS:
			
			// create position and display the caret when the window receives the keyboard focus

			// CreateCaret(hwnd,(HBITMRP)1,0,cyChar);

			CreateCaret(hwnd,NULL,0,cyChar);
			SetCaretPos(iCaretPosX , iCaretPosY * cyChar);

			ShowCaret(hwnd);

			break;	

	case WM_KILLFOCUS:

	// hide and destroy the caret when the window loses the keyboard focus

		HideCaret(hwnd);
		DestroyCaret();

		break;

	case WM_CHAR:

		// check if current location is close enough to the end of the buffer that buffer 
		// overflow may occur id 1 so , add null and display content

		if(cch > BUFSIZE - 5)
		{
			pchInputBuf[cch] = 0x00;
			SendMessage(hwnd,WM_PAINT,0,0);
		}

		switch(wParam)
		{
			case VK_BACK :   					// backspace
			case 0x0A: 	 						// linefeed
			case VK_ESCAPE :					// escape
				MessageBeep((UINT) - 1);   		// Simple beep
				break;  

			case VK_TAB :						// tab
				// convert tabs to four consecutive spaces

				for(i = 0 ; i < 4 ;i++)
					SendMessage(hwnd,WM_CHAR,VK_SPACE,0);
				break;

			case VK_RETURN :					// Carriage return

					// record the carriage return and position
					// caret at the beginning of the new line

					pchInputBuf[cch++] = VK_RETURN;
					iCaretPosX = 0;
					++iCaretPosY;
					iCurChar = cch;					
					break;		

			default :							// displayable charahcter
				

				ch = (TCHAR)wParam;
				HideCaret(hwnd);

				// retrieve the character's width and output the character

				if(cch == iCurChar)
				{
					if((int) iCaretPosX >= iLineLen)
					{
						iCaretPosX=0;
						++iCaretPosY;
						pchInputBuf[cch++] = 0x0A;      //For know the lines are ended(Linefeed/NewLine charchater)
					}	
					
					hdc = GetDC(hwnd);
					GetCharWidth32(hdc,(UINT)wParam,(UINT)wParam,&iCharWidth);
					TextOut(hdc,iCaretPosX,iCaretPosY*cyChar,&ch,1);

					ReleaseDC(hwnd,hdc);

					// store the character in the buffer
					pchInputBuf[cch++] = ch;

					iCaretPosX = iCaretPosX + iCharWidth;	

					iCurChar = cch;
				}
				ShowCaret(hwnd);
				break;
			}

		SetCaretPos(iCaretPosX,iCaretPosY*cyChar);
			
		break;

	case WM_KEYDOWN:

			switch(wParam)
			{
				
				case VK_LEFT:
				// The caret can move only to the // beginning of the current line
				// beginning of the current line
				
				HideCaret(hwnd);
				if(iCaretPosY == 0 && iCaretPosX == 0)
				{
					MessageBeep((UINT) - 1);
					ShowCaret(hwnd);
					break;
				}

				hdc = GetDC(hwnd);
				ch = pchInputBuf[--iCurChar];
				GetCharWidth32(hdc,ch,ch,&iCharWidth);
				ReleaseDC(hwnd , hdc);


				if(iCaretPosX == 0)    //  only when 
				{
					iCaretPosYTemp = 0;
					iCaretPosXTemp = 1;
					i = -1;
					iCaretPosY--;
					if(iCaretPosY != 0)   // not for first line
					{
					for(i = 0; i < cch ; i++)
					{
							if((pchInputBuf[i] == 0x0A || pchInputBuf[i] == VK_RETURN) && iCaretPosY > iCaretPosYTemp)
							{
								iCaretPosYTemp++;
								iCR++;					// counting of VK_RETURN Till Up - 1
							}
							if(iCaretPosY == iCaretPosYTemp)
								break;
					}
					}

					i++;
					while(1)
					{
						if(pchInputBuf[i] == VK_RETURN)
							break;
							
						if(pchInputBuf[i] == 0x0A)
							break;
						ch = pchInputBuf[i];
						hdc = GetDC(hwnd);
						GetCharWidth32(hdc,ch,ch,&iCharWidth);
						ReleaseDC(hwnd , hdc);
						iCaretPosX = iCaretPosX + iCharWidth;
						i++;
					}
				
				iCurChar = i - 1;
				}
				
				iCaretPosX = max(iCaretPosX - iCharWidth , 0);

		
				if(iCaretPosXTemp == 1)
				{
						if(pchInputBuf[iCurChar+1] == VK_RETURN)
						{
						if(iCaretPosX != 0)
						{
							iCaretPosX = max(iCaretPosX + iCharWidth , 0);
							iCurChar++;
						}
						iCaretPosXTemp = 0;
						}
					
				}

						
				ShowCaret(hwnd);


				break;

				case VK_RIGHT:

						if(iCurChar < cch)
						{
							HideCaret(hwnd);

							ch = pchInputBuf[iCurChar];
							chTemp = pchInputBuf[iCurChar+1];
							iVirtKey = GetKeyState(VK_SHIFT);
							if(chTemp == 0x0A)
							{
								if(iVirtKey & SHIFTED)
									{
										hdc = GetDC(hwnd);
										crPrevText = SetTextColor(hdc,RGB(255,255,255));
										crPrevBk = SetBkColor(hdc,RGB(0,0,0));
										TextOut(hdc,iCaretPosX,iCaretPosY * cyChar,&ch,1);
										SetTextColor(hdc,crPrevText);
										SetBkColor(hdc,crPrevBk);
										ReleaseDC(hwnd,hdc);
									}

								iCaretPosX = 0;
								iCaretPosY++;
								iCurChar++;
							}
							else if(ch == VK_RETURN)
							{
								iCaretPosX = 0;
								iCaretPosY++;
							}
							else
							{
								hdc = GetDC(hwnd);
								if(iVirtKey & SHIFTED)
								{
									crPrevText = SetTextColor(hdc,RGB(255,255,255));
									crPrevBk = SetBkColor(hdc,RGB(0,0,0));
									TextOut(hdc,iCaretPosX,iCaretPosY * cyChar,&ch,1);
									SetTextColor(hdc,crPrevText);
									SetBkColor(hdc,crPrevBk);
								}

								GetCharWidth32(hdc,ch,ch,&iCharWidth);
								ReleaseDC(hwnd,hdc);
								iCaretPosX = iCaretPosX + iCharWidth;
						}

					iCurChar++;
					ShowCaret(hwnd);
					break;
				}

					break;

				case VK_UP:

						HideCaret(hwnd);
						
						iVKReturnTemp = 0;
						iCaretPosXTemp = iCaretPosX;
						iCaretPosYTemp = 0;
				if(iCaretPosY == 0)
				{
					MessageBeep((UINT) - 1);
				}
				else
				{
							i = -1;
							iCaretPosY--;
							iCR = 0;
							if(iCaretPosY != 0)
							{
							for(i = 0; i < cch ; i++)
							{
								if((pchInputBuf[i] == 0x0A || pchInputBuf[i] == VK_RETURN) && iCaretPosY > iCaretPosYTemp)
								{
										iCaretPosYTemp++;
										iCR++;					// counting of VK_RETURN Till Up - 1

								}
								if(iCaretPosY == iCaretPosYTemp)
									break;
							}
							}
				
					
					i++;
					j = 0;
					iCaretPosX = 0;
					while(iCaretPosXTemp > iCaretPosX)
					{
						ch = pchInputBuf[i];
						
						if(ch == VK_RETURN || pchInputBuf[i+1] == 0x0A)
							break;
						hdc = GetDC(hwnd);
						GetCharWidth32(hdc,ch,ch,&iCharWidth);
						ReleaseDC(hwnd , hdc);
						iCaretPosX = iCaretPosX + iCharWidth;
						i++;
					}
					iCurChar = i;
				}
					ShowCaret(hwnd);	
						
						break;
				case VK_DOWN:
						
 						if(((pchInputBuf[iCurChar+1] == 0x0A || pchInputBuf[iCurChar] == VK_RETURN) && iCurChar >= cch) || iCurChar == cch)
						{
							MessageBeep((UINT) - 1);
						}
						else
						{
								iCaretPosXTemp = iCaretPosX;
								i = iCurChar;
								
								while(pchInputBuf[i] != VK_RETURN &&  i <= cch )
								{
									ch = pchInputBuf[i];
									if(ch == 0x0A)
										break;
									i++;
								}
								if(i > cch)
								{ // i chi value mothi asel jeva buffer ch limit sampl asel.
									// mhnje i<= cch ya condition mule to false zala asel 
									// varcha condition madhe
									MessageBeep((UINT) - 1);

								}
								else
								{
									iCaretPosY++;
								i++;
								
								iCaretPosX = 0;
								while(iCaretPosXTemp - 2 > iCaretPosX)
								{
									ch = pchInputBuf[i];
									if(ch == VK_RETURN || i == cch || pchInputBuf[i+1] == 0x0A)
										break;
									hdc = GetDC(hwnd);
									GetCharWidth32(hdc,ch,ch,&iCharWidth);
									ReleaseDC(hwnd , hdc);
									iCaretPosX = iCaretPosX + iCharWidth;
									i++;
								}	
								iCurChar = i;
								}
						}

						break;

				case VK_HOME :
						iCaretPosX = iCaretPosY = 0;
						iCurChar = 0;		

					break;	

				case VK_END:

				for(i = 0; i < cch ; i++)
				{
					if(pchInputBuf[i] == VK_RETURN || pchInputBuf[i] == 0x0A)
					{
						iCR++;					// count of carriage return
						iCRIndex = i+1;         // last carriage return index	
					}
				}		
				iCaretPosY = iCR;

				for(i = iCRIndex , j = 0 ; i < cch ; i++ , j++)
					szBuf[j] = pchInputBuf[i];

				szBuf[j] = TEXT('\0');

				hdc = GetDC(hwnd);
				hResult = StringCchLength(szBuf, sizeof(szBuf), &uiLength);


				if(FAILED(hResult))
				{
					// error handler
				}

				GetTextExtentPoint32(hdc,szBuf,(int)uiLength,&sz);

				iCaretPosX = sz.cx;
				ReleaseDC(hwnd,hdc);
				iCurChar = cch;

				break;

				default :
				break;
			}

			SetCaretPos(iCaretPosX,iCaretPosY*cyChar);

			break;		

	case WM_PAINT:
		
			if(cch == 0)
				break;

			hdc = BeginPaint(hwnd,&ps);

			HideCaret(hwnd);

			SetRect(&rc,0,0,iLineLen,cyClient);
			DrawText(hdc,pchInputBuf,-1,&rc,DT_LEFT);

			ShowCaret(hwnd);

	
			EndPaint(hwnd,&ps);

				break;
	
	case WM_DESTROY:
		
			PostQuitMessage(0);
		
			GlobalFree((HGLOBAL)pchInputBuf);
			UnregisterHotKey(hwnd, 0xAAAA);

			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}