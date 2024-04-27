#include<windows.h>
#include<strsafe.h>

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#define SBHEIGHT 		 17
#define SBWIDTH			 17

#define GWL_HINSTANCE       (-6) // Winuser.h

int WINAPI WinMain(
					HINSTANCE hInstance,
					HINSTANCE hPrecInstance,
					LPSTR     pszCmdLine,
					int      iCmdShow
					)
{
	int y;
	int x;
	MSG msg;
	BOOL bRet;
	HWND hwnd;
	int cxScreen;
	int cyScreen;
	WNDCLASSEX wndclass;
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
					TEXT("ALL CHILD WINDOW"),
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

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

LRESULT CALLBACK WndProc(HWND hwnd,
						UINT uiMessage,
						WPARAM wParam,
						LPARAM lParam)
{
		PAINTSTRUCT ps;
		SCROLLINFO si;
		TEXTMETRIC tm;
		HDC hdc;
		RECT rect;

		static int cxClient;   // Width of the client area
		static int cyClient;   // Height of the client area
		static int cxClientMax; // maximum width of client area

		static int cxChar; // horizontal scrolling unit 
		static int cyChar; // vertical scrolling unit 
		static int cxUpper; // average width of uppercase letters 

		 static int xPos; // current horizontal scrolling position 
		 static int yPos; // current vertical scrolling position 

		 int i; // loop counter 
		 int x,y; // horizontal and vertical coordinates

		 int LastLine; // last line in the invalidated area 
		 int FirstLine; // first line in the invalidated area 


		HRESULT hr;
		size_t Length; // length of an abc[] item 

		static HWND hwndHScroll; // handle of horizontal scrollbar
		static HWND hwndVScroll; // handle of vertical scrollbar

		static BOOLEAN bHHideFlag; // flag is TRUE if horizontal scrollbar is hidden
		static BOOLEAN bVHideFlag; // flag is TRUE if vertical scrollbar is hidden


 		// Create an array of lines to display. 
		#define LINES 28
		static TCHAR *arrText[] = {
		TEXT("Ajinkyatara"), TEXT("Bhairavgad"), TEXT("Chandan"),
		TEXT("Dhak Bahiri"), TEXT("Elephant"), TEXT("Fatte gad"),
		TEXT("Gowalkot"), TEXT("Harishchandragad"), TEXT("Indrai"),
		TEXT("Janjira"), TEXT("Korigad"), TEXT("Lohagad"),
		TEXT("Mahuli"), TEXT("Narayangad"), TEXT("Pratapgad"),
		TEXT("Panhala"), TEXT("Purandar"), TEXT("Raigad"),
		TEXT("Shivneri"), TEXT("Torna"), TEXT("Underi"),
		TEXT("Vishalgad"), TEXT("Worli"), TEXT("Rajgad"),
		TEXT("Yashwantgad"), TEXT("Rohida"),
		TEXT("These are alphabetical fort names."),
		TEXT("")
		};

		
		

		switch(uiMessage)
		{	
			case WM_CREATE:
			
				hdc = GetDC(hwnd);

				GetTextMetrics (hdc, &tm);

				cxChar = tm.tmAveCharWidth;
				cxUpper = (tm.tmPitchAndFamily & 1 ? 3 : 2) * cxChar/2;
				cyChar = tm.tmHeight + tm.tmExternalLeading;

				ReleaseDC(hwnd,hdc);

				// Set an arbitrary maximum width for client area. 
				// (cxClientMax is the sum of the widths of 48 average 
				// lowercase letters and 12 uppercase letters.) 
				cxClientMax = 48 * cxChar + 12 * cxUpper;

				GetClientRect(hwnd, &rect);

				hwndHScroll = CreateWindow(
											TEXT("SCROLLBAR"),
											NULL,
											WS_CHILD | WS_VISIBLE | SBS_HORZ,
											rect.left,                  // horizontal position 
											rect.bottom -  SBHEIGHT,    // vertical position 
											rect.right - SBWIDTH,       // width of the scroll bar
											SBHEIGHT,                    // height of the scroll bar
											hwnd,
											(HMENU)NULL,
											(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
											NULL
											);

				hwndVScroll = CreateWindow(
											TEXT("SCROLLBAR"),
											NULL,
											WS_CHILD | WS_VISIBLE | SBS_VERT,
											rect.right - SBWIDTH,         // horizontal position 
											rect.top,    // vertical position 
											SBWIDTH,       // width of the scroll bar
											rect.bottom - SBHEIGHT,                    // height of the scroll bar
											hwnd,
											(HMENU)NULL,
											(HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
											NULL
											);

				SetScrollRange (hwndHScroll, SB_CTL, 0, 2 + cxClientMax / cxChar, FALSE) ;
				SetScrollPos (hwndHScroll, SB_CTL, 0, FALSE) ;

				SetScrollRange (hwndVScroll, SB_CTL, 0, LINES - 1, FALSE) ;
				SetScrollPos (hwndVScroll, SB_CTL, 0, FALSE) ;

				ShowScrollBar(hwndHScroll, SB_CTL, FALSE);
				ShowScrollBar(hwndVScroll, SB_CTL, FALSE);

				bVHideFlag = TRUE;
 				bHHideFlag = TRUE;
				
				break;
			
			case WM_SIZE:

				// Retrieve the dimensions of the client area. 
				cxClient = LOWORD (lParam);
				cyClient = HIWORD (lParam);

				 if(bVHideFlag == TRUE && (cyClient/cyChar) < (LINES - 1))
				 {
					 ShowScrollBar(hwndVScroll, SB_CTL, TRUE);
					 bVHideFlag = FALSE;
				 }
				 else if(bVHideFlag == FALSE && (cyClient/cyChar) >= (LINES - 1))
				 {
					//Code From MSDN Simplified by Swapnil Y. Mane 9403967926
					 ShowScrollBar(hwndVScroll, SB_CTL, FALSE);
					 bVHideFlag = TRUE;
				 }

				 if(bVHideFlag == FALSE)
				{
					 // Set the vertical scrolling range and page size
					si.cbSize = sizeof(si);
					si.fMask = SIF_RANGE | SIF_PAGE | SIF_POS;
					si.nMin = 0;
					si.nMax = LINES - 1;
					si.nPage = cyClient / cyChar;
					si.nPos = 0;
					SetScrollInfo(hwndVScroll, SB_CTL, &si, TRUE);
					MoveWindow(hwndVScroll, cxClient - SBWIDTH, 0,SBWIDTH, cyClient - SBHEIGHT, TRUE);
				}

				 if(bHHideFlag == TRUE && (cxClient/cxChar) < (2 + cxClientMax / cxChar))
				 {
					ShowScrollBar(hwndHScroll, SB_CTL, TRUE);
					bHHideFlag = FALSE;
				 }
				else if(bHHideFlag == FALSE && (cxClient/cxChar) >= (2 + cxClientMax / cxChar))
				 {
					ShowScrollBar(hwndHScroll, SB_CTL, FALSE);
					bHHideFlag = TRUE;
				}

				if(bHHideFlag == FALSE)
				{
					 // Set the horizontal scrolling range and page size. 
					 si.cbSize = sizeof(si);
					si.fMask = SIF_RANGE | SIF_PAGE;
					si.nMin = 0;
					si.nMax = 2 + cxClientMax / cxChar;
					si.nPage = cxClient / cxChar;
					si.nPos = 0;
					SetScrollInfo(hwndHScroll, SB_CTL, &si, TRUE);

					MoveWindow(hwndHScroll, 0, cyClient - SBHEIGHT,
					cxClient - SBWIDTH, SBHEIGHT, TRUE);
				}
	
				
				break;
		
			
			case WM_HSCROLL:

				// Get all the vertial scroll bar information.
				si.cbSize = sizeof (si);
				si.fMask = SIF_ALL;

				// Save the position for comparison later on.
				 GetScrollInfo (hwndHScroll, SB_CTL, &si);
				 xPos = si.nPos;

				 switch (LOWORD (wParam))
				{
					 // User clicked the left arrow.
					 case SB_LINELEFT:
					 si.nPos = si.nPos - 1;
					 break;

					 // User clicked the right arrow.
					case SB_LINERIGHT:
					 si.nPos = si.nPos + 1;
					break;

					//Code From MSDN Simplified by Swapnil Y. Mane 9403967926
					 // User clicked the scroll bar shaft left of the scroll box.
					 case SB_PAGELEFT:
					si.nPos = si.nPos - si.nPage;
					 break;

					// User clicked the scroll bar shaft right of the scroll box.
					 case SB_PAGERIGHT:
					si.nPos = si.nPos + si.nPage;
					 break;

					 // User dragged the scroll box.
					 case SB_THUMBTRACK:
					 si.nPos = si.nTrackPos;
					 break;

				 default :
				 break;
				 }

				// Set the position and then retrieve it. Due to adjustments
				// by Windows it may not be the same as the value set.
				si.fMask = SIF_POS;
				SetScrollInfo (hwndHScroll, SB_CTL, &si, TRUE);
				GetScrollInfo (hwndHScroll, SB_CTL, &si);

				// If the position has changed, scroll the window.
				if (si.nPos != xPos)
				{
					ScrollWindow(hwndHScroll, cxChar * (xPos - si.nPos), 0, NULL, NULL);
					InvalidateRect (hwnd, NULL, TRUE) ;
				}

 			break;

			case WM_VSCROLL:

				 // Get all the vertial scroll bar information.
				si.cbSize = sizeof (si);
				si.fMask = SIF_ALL;
				GetScrollInfo (hwndVScroll, SB_CTL, &si);

				// Save the position for comparison later on.
				 yPos = si.nPos;
				
				 switch (LOWORD (wParam))
				 {
					 // User clicked the HOME keyboard key.
					 case SB_TOP:
					 si.nPos = si.nMin;
					 break;
					 // User clicked the END keyboard key.
					 case SB_BOTTOM:
					 si.nPos = si.nMax;
					 break;

					// User clicked the top arrow.
					 case SB_LINEUP:
					 si.nPos = si.nPos - 1;
					break;
					 // User clicked the bottom arrow.
					 case SB_LINEDOWN:
					 si.nPos = si.nPos + 1;
					 break;

					 // User clicked the scroll bar shaft above the scroll box.
					 case SB_PAGEUP:
					//Code From MSDN Simplified by Swapnil Y. Mane 9403967926
					 si.nPos = si.nPos - si.nPage;
					 break;

					 // User clicked the scroll bar shaft below the scroll box.
					 case SB_PAGEDOWN:
					 si.nPos = si.nPos + si.nPage;
					 break;

					 // User dragged the scroll box.
					 case SB_THUMBTRACK:
					 si.nPos = si.nTrackPos;
					 break;

					default:
				 break;
				 }

				 // Set the position and then retrieve it. Due to adjustments
				 // by Windows it may not be the same as the value set.
				si.fMask = SIF_POS;
				 SetScrollInfo (hwndVScroll, SB_CTL, &si, TRUE);
				GetScrollInfo (hwndVScroll, SB_CTL, &si);

				 // If the position has changed, scroll window and update it.
				 if (si.nPos != yPos)
				 {
				 ScrollWindow(hwndVScroll, 0, cyChar * (yPos - si.nPos), NULL, NULL);
				 InvalidateRect (hwnd, NULL, TRUE) ;
				 }
				 break;	

			case WM_PAINT :
			
				 // Prepare the window for painting.
				 hdc = BeginPaint (hwnd, &ps);

				// Get vertical scroll bar position.
				si.cbSize = sizeof (si);
				si.fMask = SIF_POS;
				 GetScrollInfo (hwndVScroll, SB_CTL, &si);
				yPos = si.nPos;

				// Get horizontal scroll bar position.
				 GetScrollInfo (hwndHScroll, SB_CTL, &si);
				xPos = si.nPos;

				 // Find painting limits.
				 FirstLine = max (0, yPos + ps.rcPaint.top / cyChar);
				 LastLine = min (LINES - 1, yPos + ps.rcPaint.bottom / cyChar);

				 for (i = FirstLine; i <= LastLine; i++)
				 {
				x = cxChar * (1 - xPos);
				 y = cyChar * (i - yPos);

				 // Note that "55" in the following depends on the 
				 // maximum size of an abc[] item. Also, you must include
				// strsafe.h to use the StringCchLength function.
				 hr = StringCchLength(arrText[i], 55, &Length);
				 if ((FAILED(hr))|(Length == 0))
				 {
				//
				// write error handler
				//
				 }

				// Write a line of text to the client area.
				//Code From MSDN Simplified by Swapnil Y. Mane 9403967926
				 TextOut(hdc, x, y, arrText[i], (int)Length);
				 }

				// Indicate that painting is finished.
				EndPaint (hwnd, &ps);

			break;

			case WM_KEYDOWN:
			 {
				 WORD wScrollNotify = 0xFFFF;
				 switch (wParam)
				 {
				case VK_UP:
				wScrollNotify = SB_LINEUP;
				 break;

				 case VK_PRIOR:
				 wScrollNotify = SB_PAGEUP;
				 break;

				 case VK_NEXT:
				 wScrollNotify = SB_PAGEDOWN;
				 break;

				 case VK_DOWN:
				 wScrollNotify = SB_LINEDOWN;
				 break;

				 case VK_HOME:
				 wScrollNotify = SB_TOP;
				 break;

				 case VK_END:
				wScrollNotify = SB_BOTTOM;
				break;
				}

				if (wScrollNotify != -1)
				 SendMessage(hwnd, WM_VSCROLL, MAKELONG(wScrollNotify, 0), 0L);
			}
				 break;
			

			case WM_DESTROY:
				
				PostQuitMessage(0);
				
			    break;
		}
		return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
