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

#define GCL_HBRBACKGROUND (-10) // Winuser.h

#define GWL_HINSTANCE       (-6) // Winuser.h
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
                    TEXT("RADIO BUTTON ON DIALOG"),
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
				case ID_DIALOG_MODAL:
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
						MessageBox(hwnd,TEXT("Use of Radio Button") TEXT("\n\n\t\t\t\t-Aishwarya"),szAppName , MB_OK);
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

	HWND hEditBoxDecimal,hEditBoxHex;
	int iLength;
	TCHAR szText[255]={TEXT("255")};
	static int number;
	int a[100];
	int iCounter,remainder,count=0;
	int num;
	static TCHAR stringDest[30]={TEXT("")},stringSource[30];

	switch(uiMessage)
	{
			
	  case WM_INITDIALOG:

			return TRUE;		
			
	  case WM_COMMAND:
				
			switch(LOWORD(wParam))
			{
					case WM_DESTROY:
						EndDialog(hwnd,ID_BTN_CANCEL);

					case ID_BTN_CANCEL:
						EndDialog(hwnd,ID_BTN_CANCEL);
						return TRUE;
							
					case ID_BTN_CONVERT:
						memset(stringDest,0, 30*sizeof(TCHAR));
						hEditBoxHex = GetDlgItem(hwnd,IDC_EDIT2);
						SendMessage(hEditBoxHex,EM_SETMODIFY,0,(LPARAM)stringDest);
						SetWindowText(hEditBoxHex,stringDest);

						hEditBoxDecimal = GetDlgItem(hwnd,IDC_EDIT1);
						iLength = SendMessage(hEditBoxDecimal,EM_GETLINE,0,(LPARAM)szText);
						szText[iLength]='\0';
						sscanf(szText, "%d", &number);
						for(iCounter=0;number!=0;iCounter++)
						{
							remainder=number%16;
							number=number/16;
							a[iCounter]=remainder;
							count++;
						}
				
				    for(iCounter=count-1;iCounter>=0;iCounter--)
					{
						switch(a[iCounter])
							{
						case 10:
								wsprintf(stringSource,TEXT("a"));
								strcat(stringDest,"a");
							  break;
						case 11:
								wsprintf(stringSource,TEXT("b"));
								strcat(stringDest,TEXT("b"));
							 break;
						case 12:
								wsprintf(stringSource,TEXT("c"));
								strcat(stringDest,TEXT("c"));
							 break;
						case 13:
								wsprintf(stringSource,TEXT("d"));
								strcat(stringDest,TEXT("d"));
							 break;
						case 14:
								wsprintf(stringSource,TEXT("e"));
								strcat(stringDest,stringSource);
							 break;
						case 15:
								wsprintf(stringSource,TEXT("f"));
								strcat(stringDest,TEXT("f"));
							break;
						default:
							wsprintf(stringSource,TEXT("%d"),a[iCounter]);
							strcat(stringDest,stringSource);
						}
					}
						SendMessage(hEditBoxHex,EM_SETMODIFY,0,(LPARAM)stringDest);
						SetWindowText(hEditBoxHex,stringDest);
						break;

			}
		  break;
	}
	return FALSE;
}