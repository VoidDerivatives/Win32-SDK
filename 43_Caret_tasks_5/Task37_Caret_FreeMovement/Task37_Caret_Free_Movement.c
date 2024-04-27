#include<windows.h>

#ifdef _LOGGER_
	#include"logger.h"
	#include<stdio.h>
#endif   //_LOGGER_

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
void RandomRectangle(HWND);

#define WINDOW_WIDTH	800
#define WINDOW_HEIGHT   600

#ifdef _LOGGER_
	FILE *fp = NULL;
#endif   //_LOGGER_


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

#if MAIN_LOG
	fp = fopen("Log.txt","w");
	if(fp == NULL)
		return -1;
#endif   //MAIN_LOG


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
					TEXT("CARET FREE MOVEMENT"),
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
#endif   //MAIN_LOG

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
	TEXTMETRIC tm;
	RECT rect;

	static int cyChar;
	static int cxChar;
	static int cxCaps;
	static int iNoOfRows;
	
	static int iXPos;
	static int iYPos;

	static int iLines;
	static int iLinesTemp;
	static int iLineLen;

	static BOOL bMiddle = 1;

	UINT uiCurrentBlinkTime = 0; 
	
	TCHAR tText[50];
	
	static int cxClient;
	static int cyClient;
	
	switch(uiMessage)
	{
	case WM_CREATE:
			hdc = GetDC(hwnd);

			GetTextMetrics(hdc,&tm);

			cyChar = tm.tmHeight;
			cxChar = tm.tmAveCharWidth;

			/*if((tm.tmPitchAndFamily & 1) != 0)
			{
				cxCaps = 3 * cxChar / 2; // Captical
			}
			else
			{*/
				cxCaps = cxChar;                        // Width of character
			//}

			ReleaseDC(hwnd,hdc);

			break;		
	case WM_SIZE:

			cxClient = LOWORD(lParam);
			cyClient = HIWORD(lParam);

			bMiddle = 1;

			// Calculate the maximum width of a line and the 
			// maximum number of lines in the client area. 

			iLineLen = cxClient / cxChar;
			iLines = cyClient / cyChar;
			break;
	
	case WM_SETFOCUS:

			CreateCaret(hwnd,NULL,0,10);

			SetCaretPos(cxClient/2,cyClient/2);

			ShowCaret(hwnd);
			
		break;
	
	case WM_KILLFOCUS:

		  DestroyCaret();

		break;

	case WM_PAINT:

			HideCaret(hwnd);

			hdc = BeginPaint(hwnd,&ps);
 			
 			//HideCaret(hwnd);

 			if(bMiddle == 1)
 			{
 				iXPos = cxClient / 2;
 				iYPos = cyClient / 2;
 				iLinesTemp = iLines / 2;
			}
			
			
			SetCaretPos(iXPos,iYPos);	
			bMiddle = 0;
			ShowCaret(hwnd);

			EndPaint(hwnd,&ps);

				break;
	
	case WM_KEYDOWN:

		switch(wParam)
		{
			case VK_LEFT:
				
				if(iXPos > cxCaps)               // 10 ne pudhe
				{
					iXPos = iXPos - cxCaps;
				}
				else
				{	
					
					if(iLinesTemp == 0)
					{
						MessageBeep((UINT) -1);	
						break;
					}
					iLinesTemp--;
					iXPos = cxClient - cxCaps;   // 1 width ch anatr right pasun
					iYPos = iYPos - cyChar;      // 1 hight ne varti gela
				} 

				#if WND_PROC_LEFT
					fprintf(fp, "Value of iXPos :%d\n", iXPos);
					fprintf(fp, "Value of iYPos :%d\n", iYPos);
					fprintf(fp, "Value of cxCaps :%d\n", cxChar);
					fprintf(fp, "Value of cyChar :%d\n", cyChar);
						flushall();
				#endif  //WND_PROC_LEFT
				
				
				InvalidateRect(hwnd,NULL,TRUE);

			break;
			
			case VK_RIGHT:
				
				if(iXPos < cxClient - cxCaps)     //10 ne kami
				{
					iXPos = iXPos + cxCaps;
				}
				else
				{
					if(((iLinesTemp+1) == iLines && (iXPos == (cxClient - cxCaps))) )
					{
						MessageBeep((UINT) -1);	
						break;
					}
					iLinesTemp++;
					iXPos = cxCaps;
					iYPos = iYPos + cyChar;
				}

				#if WND_PROC_RIGHT
					fprintf(fp, "Value of iXPos :%d\n", iXPos);
					fprintf(fp, "Value of iYPos :%d\n", iYPos);
					fprintf(fp, "Value of cxCaps :%d\n", cxChar);
					fprintf(fp, "Value of cyChar :%d\n", cyChar);
					fprintf(fp, "Value of iLinesTemp :%d\n", iLinesTemp);

						flushall();
				#endif  //WND_PROC_RIGHT
				
				
				InvalidateRect(hwnd,NULL,TRUE);
			break;

			case VK_UP:

				if(--iLinesTemp >= 0)
				{
					iYPos = iYPos - cyChar;
					//iLinesTemp--;
				}
				else
				{
						iLinesTemp++;
					MessageBeep((UINT) -1);	
				}
				#if WND_PROC_UP
					fprintf(fp, "Value of iXPos :%d\n", iXPos);
					fprintf(fp, "Value of iYPos :%d\n", iYPos);
					fprintf(fp, "Value of cxCaps :%d\n", cxChar);
					fprintf(fp, "Value of cyChar :%d\n", cyChar);
					fprintf(fp, "Value of iLinesTemp :%d\n", iLinesTemp);
					flushall();
				#endif  //WND_PROC_UP
				
				InvalidateRect(hwnd,NULL,TRUE);
			break;

			case VK_DOWN:

				if(++iLinesTemp != iLines)
				{
					iYPos = iYPos + cyChar;
					//iLinesTemp++;
				}
				else
				{
					iLinesTemp--;
					MessageBeep((UINT) -1);
				}
				#if WND_PROC_DOWN
					fprintf(fp, "Value of iXPos :%d\n", iXPos);
					fprintf(fp, "Value of iYPos :%d\n", iYPos);
					fprintf(fp, "Value of cxCaps :%d\n", cxChar);
					fprintf(fp, "Value of cyChar :%d\n", cyChar);
					flushall();
				#endif  //WND_PROC_DOWN
				
				InvalidateRect(hwnd,NULL,TRUE);
			break;
		}
		break;
	case WM_DESTROY:
			
			PostQuitMessage(0);
			break;

	}
	return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}