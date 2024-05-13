#include<windows.h>
#include"resource.h"
#include<tchar.h>

#define ID_TIMER    1

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

//#define GCL_HBRBACKGROUND (-10) // Winuser.h
#define GWL_HINSTANCE (-10) 
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
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU1);
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
                    TEXT("PUSH BUTTON ON DIALOG"),
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
	int iRet;
	HINSTANCE hInstance;

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
                    MessageBox(hwnd,TEXT("Use of Push Button") TEXT("\n\n\t\t\t\t-Aishwarya"),szAppName , MB_OK);
                    break;
		}
		break;

      case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

BOOL CALLBACK ModalDlgProc(HWND hDlg,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
	HWND hStaticBtnText;
	switch(uiMessage)
	{
		case WM_INITDIALOG:
			return TRUE;
		case WM_COMMAND:
			
			switch(LOWORD(wParam))
			{

				case IDC_BTN_CANCEL:
					EndDialog(hDlg,IDC_BTN_CANCEL);
					return TRUE;

				case IDC_BTN_RED:
				case IDC_BTN_GREEN:
				case IDC_BTN_BULE:
					hStaticBtnText = GetDlgItem(hDlg,IDC_STATIC_TEXT);
					switch(HIWORD(wParam))
					{
					case BN_CLICKED:
						if(LOWORD(wParam) == IDC_BTN_RED)
							SetWindowText(hStaticBtnText,TEXT("Button RED Pushed"));
						else if(LOWORD(wParam) == IDC_BTN_GREEN)
							SetWindowText(hStaticBtnText,TEXT("Button GREEN Pushed"));
						else if(LOWORD(wParam) == IDC_BTN_BULE)
							SetWindowText(hStaticBtnText,TEXT("Button BLUE Pushed"));
					}
					return TRUE;
					
				case WM_DESTROY:
						EndDialog(hDlg,IDC_BTN_CANCEL);
			}
			break;

	}
	return FALSE;
}
