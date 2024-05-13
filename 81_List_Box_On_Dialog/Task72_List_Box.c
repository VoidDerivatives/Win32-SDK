#include<windows.h>
#include"resource.h"
#include<tchar.h>
#include<stdio.h>

#define ID_TIMER    1

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define GWL_HINSTANCE       (-6) // Winuser.h
#define GCL_HBRBACKGROUND (-10) // Winuser.h
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);
BOOL CALLBACK ModalDlgProc(HWND,UINT,WPARAM,LPARAM);

HANDLE g_hDlgModeless;

TCHAR szAppName[] = TEXT("PushButton Task");

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
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(BLACK_BRUSH);
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);;
    wndclass.cbClsExtra = 0;
    wndclass.cbWndExtra = 0;
    wndclass.hIconSm = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));

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
                    TEXT("LIST BOX ON DIALOG"),
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
	HINSTANCE hInstance;
	int iRet;

    switch(uiMessage)
    {               
	  case WM_COMMAND:
		switch(LOWORD(wParam))
		{
				case ID_DIALOGBOX_MODAL:
					InvalidateRect(hwnd,NULL,TRUE);
				
					hInstance = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);

					iRet = DialogBox(
									hInstance,
									 MAKEINTRESOURCE(IDD_DIALOG1),
									 hwnd,
									 (DLGPROC)ModalDlgProc
									);

					break;

				case ID_HELP_HELP:
						MessageBox(hwnd,TEXT("Help Not yet Implimented!"),szAppName,MB_OK);
						break;  

				case ID_HELP_ABOUT:
						MessageBox(hwnd,TEXT("Use of List Box") TEXT("\n\n\t\t\t\t-Aishwarya"),szAppName , MB_OK);
						break;
		}
		break;

      case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}
BOOL CALLBACK ModalDlgProc(HWND hwnd,UINT uiMessage,WPARAM wParam,LPARAM lParam)
{
	static HWND hwndCheckBox[3];
	static HWND hwndSubmitBtn,hwndListBox,hwndClearBtn;
	int iCounter;
	static BOOL bFavColor[3];
	static TCHAR String[30]={TEXT("")};
	static BOOL bFlag = FALSE;
	
	switch(uiMessage)
	{
			
	  case WM_INITDIALOG:
	  		hwndCheckBox[0] = GetDlgItem(hwnd,IDC_CHECK_RED);
			hwndCheckBox[1] = GetDlgItem(hwnd,IDC_CHECK_GREEN);
			hwndCheckBox[2] = GetDlgItem(hwnd,IDC_CHECK_BLUE);
			hwndListBox = GetDlgItem(hwnd,IDC_LIST_COLOR);
			hwndSubmitBtn=GetDlgItem(hwnd,ID_BTN_SUBMIT);
			hwndClearBtn = GetDlgItem(hwnd,IDC_BTN_CLEAR);

			memset(String,0, 30*sizeof(TCHAR));

			return TRUE;		
			
	  case WM_COMMAND:
				
				switch(LOWORD(wParam))
				{
					case WM_DESTROY:
						EndDialog(hwnd,ID_BTN_CANCEL);

					case ID_BTN_CANCEL:
						EndDialog(hwnd,ID_BTN_CANCEL);
						return TRUE;

					case IDC_BTN_CLEAR:
						
						if(bFlag == TRUE)
						{
							for(iCounter = 0;iCounter<3;iCounter++)
							{
								SendMessage(hwndCheckBox[iCounter],BM_SETCHECK,0,0);
								bFavColor[iCounter] = 0;
							}

							SendMessage(hwndListBox,LB_RESETCONTENT,0,0);
							ShowWindow(hwndSubmitBtn,SW_NORMAL);
							ShowWindow(hwndClearBtn,SW_HIDE);
							bFlag = FALSE;
						}
						return TRUE;
					case ID_BTN_SUBMIT:

						
						for(iCounter = 0;iCounter<3;iCounter++) // Setting
						{
							if(SendMessage(hwndCheckBox[iCounter],BM_GETSTATE,0,0))
							{
								bFlag = TRUE;
								bFavColor[iCounter] = 1;
							}
						}

						if(bFlag == TRUE)
						{
							ShowWindow(hwndSubmitBtn,SW_HIDE);
							ShowWindow(hwndClearBtn,SW_NORMAL);
						}

				
							if(bFavColor[2])
							{
								wsprintf(String,TEXT("BLUE"));
								SendMessage(hwndListBox,LB_ADDSTRING,0,(LPARAM)String);
								memset(String,0, 30*sizeof(TCHAR));
							}
							if(bFavColor[1])
							{
								wsprintf(String,TEXT("GREEN"));
								SendMessage(hwndListBox,LB_ADDSTRING,0,(LPARAM)String);
								memset(String,0, 30*sizeof(TCHAR));
							}
							if(bFavColor[0])
							{	
								wsprintf(String,TEXT("RED"));
								//5strcpy(String,TEXT("RED"));
								SendMessage(hwndListBox,LB_ADDSTRING,0,(LPARAM)String);
								memset(String,0, 30*sizeof(TCHAR));	
							}
						
					break;
							
				}
				
			
				break;
	}
	return FALSE;
}