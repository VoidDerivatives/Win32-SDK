#include<windows.h>

// changing System Menu
#define IDM_SYS_ABOUT   1
#define IDM_SYS_HELP    2
#define IDM_SYS_REMOVE  3
#define IDM_SYS_MYCLOSE 4
// Manual memu
#define IDM_HELP_HELP   5
#define IDM_HELP_ABOUT  6

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define GCL_HBRBACKGROUND (-10) // Winuser.h
LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

TCHAR szAppName[] = TEXT("MENU");   
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
    HMENU hMenu,hMenu1,hMenuPopup;   // Chaning system menu
    int cxScreen;
    int cyScreen;
    int x;
    int y;

    TCHAR szAppName[] = TEXT("System Menu change");

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

            // Manual Making menu'
    hMenu1 = CreateMenu();
    hMenuPopup = CreateMenu();
    AppendMenu(hMenuPopup,MF_STRING,IDM_HELP_HELP,"H&elp");
    AppendMenu(hMenuPopup,MF_STRING,IDM_HELP_ABOUT,"&About");
    AppendMenu(hMenu1,MF_POPUP,(UINT_PTR )hMenuPopup,"&Help"); 

    hwnd = CreateWindow(
                    szAppName,
                    TEXT("SYSTEM MENU CHANGE"),
                    WS_OVERLAPPEDWINDOW,
                    x,
                    y,
                    WINDOW_WIDTH,
                    WINDOW_HEIGHT,
                    NULL,
                    hMenu1,
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
    AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
    AppendMenu(hMenu,MF_STRING,IDM_SYS_MYCLOSE,TEXT("Close Window ..."));

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

    switch(uiMessage)
    {               
        case WM_COMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_HELP_HELP:
                     MessageBox(hwnd,TEXT("Help Not yet Implimented!"),szAppName,MB_ICONWARNING | MB_OK);
                  
                    break;
                case IDM_HELP_ABOUT:
                    MessageBox(hwnd,TEXT("Without '.rc' and 'resource.h' program has been Made!!!") TEXT("\n\n\t\t\t\t\t-Aishwarya"),szAppName ,MB_OK);
                    
                    break;
            }

            break;
        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_SYS_HELP:
                  MessageBox(hwnd,TEXT("Help Not Implimented yet"),szAppName,MB_ICONWARNING | MB_OK);
                    
                break;
                case IDM_SYS_ABOUT:
                MessageBox(hwnd,TEXT("A system Menu Change Program!") TEXT("(c)Charles Petzold , 1998"),szAppName,MB_ICONEXCLAMATION | MB_OK);
                    
                    break;
                case IDM_SYS_REMOVE:
                    GetSystemMenu(hwnd,TRUE);
                    break;

                case IDM_SYS_MYCLOSE:
                    PostQuitMessage(0);
                    break;    
            }      
            break;
        case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}