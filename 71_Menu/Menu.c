#include<windows.h>
#include"resource.h"

#define ID_TIMER    1

// changing System Menu
#define IDM_SYS_ABOUT   1
#define IDM_SYS_HELP    2
#define IDM_SYS_REMOVE  3

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define GCL_HBRBACKGROUND (-10) // Winuser.h
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

TCHAR szAppName[] = TEXT("MenuDemo");

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
    HMENU hMenu;   // Chaning system menu
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
    wndclass.lpszMenuName = MAKEINTRESOURCE(IDR_MENU);
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
                    TEXT("MENU"),
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

    //changing system Menu
    hMenu = GetSystemMenu(hwnd,FALSE);
    AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
    AppendMenu(hMenu,MF_STRING,IDM_SYS_ABOUT,TEXT("About..."));
    AppendMenu(hMenu,MF_STRING,IDM_SYS_HELP,TEXT("Help..."));
    AppendMenu(hMenu,MF_STRING,IDM_SYS_REMOVE,TEXT("Remove Additions"));

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
    HMENU hMenu;
    static int iSelection = IDM_BACKGROUND_WHITE;
    static int idColor[5] = {WHITE_BRUSH,LTGRAY_BRUSH,GRAY_BRUSH,DKGRAY_BRUSH,BLACK_BRUSH};

    POINT point;
    HINSTANCE hInstance;
    static HMENU hMenuPopup;

    switch(uiMessage)
    {               
        case WM_COMMAND:

            hMenu = GetMenu(hwnd);
            switch(LOWORD(wParam))
            {
                case IDM_FILE_NEW:
                case IDM_FILE_OPEN:          
                case IDM_FILE_SAVE:         
                case IDM_FILE_SAVEAS:
                    MessageBeep(0);

                   break;
                
                case IDM_FILE_EXIT:
                    SendMessage(hwnd,WM_CLOSE,0,0);
                    break;

                case IDM_EDIT_UNDO:
                case IDM_EDIT_CUT:
                case IDM_EDIT_COPY:
                case IDM_EDIT_PASTE:
                case IDM_EDIT_DELETE:
                     MessageBeep(0);
                     break;         

                case IDM_BACKGROUND_WHITE:
                case IDM_BACKGROUND_LIGHTGRAY:
                case IDM_BACKGROUND_GRAY:
                case IDM_BACKGROUND_DARKGRAY:
                case IDM_BACKGROUND_BLACK:
                    CheckMenuItem(hMenu,iSelection,MF_UNCHECKED);  
                    iSelection = LOWORD(wParam);  
                    CheckMenuItem(hMenu,iSelection,MF_CHECKED);  
                    SetClassLong(hwnd,GCL_HBRBACKGROUND,(LONG)GetStockObject(idColor[LOWORD(wParam)-IDM_BACKGROUND_WHITE]));
                    InvalidateRect(hwnd,NULL,TRUE);
                    break;

                case IDM_TIMER_START:
                    if(SetTimer(hwnd,ID_TIMER,1000,NULL)) 
                    {
                        EnableMenuItem(hMenu,IDM_TIMER_START,MF_GRAYED);
                        EnableMenuItem(hMenu,IDM_TIMER_STOP,MF_ENABLED);
                    }
                    break;

                case IDM_TIMER_STOP:
                    KillTimer(hwnd,ID_TIMER);
                    EnableMenuItem(hMenu,IDM_TIMER_START,MF_ENABLED);
                    EnableMenuItem(hMenu,IDM_TIMER_STOP,MF_GRAYED);
                    break;

                case IDM_HELP_HELP:
                    MessageBox(hwnd,TEXT("Help Not yet Implimented!"),szAppName,MB_ICONEXCLAMATION | MB_OK);
                    break;  

                case IDM_HELP_ABOUTMENUDEMO:
                    MessageBox(hwnd,TEXT("Menu Demo") TEXT("(c)Charles Petzold , 1998"),szAppName ,MB_ICONEXCLAMATION | MB_OK);
                    break;  
            }
            break;

        case WM_TIMER:
            MessageBeep(0);    

            break;

        case WM_CREATE:
            hInstance = ((LPCREATESTRUCT)lParam)->hInstance;
            hMenuPopup = LoadMenu(hInstance,MAKEINTRESOURCE(IDR_MENU));
            hMenuPopup = GetSubMenu(hMenuPopup,0);
            break;    

        case WM_RBUTTONUP:
            point.x=LOWORD(lParam);
            point.y=HIWORD(lParam);
            ClientToScreen(hwnd,&point);
            TrackPopupMenu(hMenuPopup,TPM_RIGHTBUTTON,point.x,point.y,0,hwnd,NULL);
            break;  

        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_SYS_HELP:
                  MessageBox(hwnd,TEXT("Help Not Implimented yet"),szAppName,MB_ICONEXCLAMATION | MB_OK);
                    
                break;
                case IDM_SYS_ABOUT:
                MessageBox(hwnd,TEXT("A system Menu Change Program!") TEXT("(c)Charles Petzold , 1998"),szAppName,MB_ICONEXCLAMATION | MB_OK);
                    
                    break;
                case IDM_SYS_REMOVE:
                    GetSystemMenu(hwnd,TRUE);
                    break;
            }      
            break;
        case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}