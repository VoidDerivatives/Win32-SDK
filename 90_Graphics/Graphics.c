#include<windows.h>
#include<stdio.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")


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
    hwnd = CreateWindow(
                    szAppName,
                    TEXT("MOUSE STATUS"),
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

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

FILE *fp = NULL;
LRESULT CALLBACK WndProc(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static POINT ptPrevious;
    HPEN hPen;
    char szText[255];
    
    switch(uiMessage)
    {       
            
    case WM_CREATE:
            fp = fopen("demo.txt","r");
            if(fp == NULL)
            {
                return -1;
            }
            break;
    case WM_PAINT:
            hdc  = BeginPaint(hwnd,&ps);
          
            EndPaint(hwnd,&ps);
            break;

    case WM_TIMER:
            
            fgets(szText,sizeof(szText),fp);
            sscanf(szText, "%d", &ptPrevious.x);

            fgets(szText,sizeof(szText),fp);
            sscanf(szText, "%d", &ptPrevious.y);
            
                 hdc = GetDC(hwnd);
        
                MoveToEx(hdc,ptPrevious.x,ptPrevious.y,NULL);

                hPen = CreatePen(PS_DASHDOT , 1 , RGB(255,255,255));
                SelectObject(hdc,hPen);
                
                fgets(szText,sizeof(szText),fp);
                sscanf(szText, "%d", &ptPrevious.x);

                fgets(szText,sizeof(szText),fp);
                sscanf(szText, "%d", &ptPrevious.y);
            
                LineTo(hdc,ptPrevious.x , ptPrevious.y);
              
                DeleteObject(hPen);
              
                ReleaseDC(hwnd,hdc);

        break;
    
    case WM_LBUTTONDOWN:
             SetTimer(hwnd,1,10,NULL);

         break;             

    case WM_DESTROY:
                fclose(fp);
                fp = NULL;
                KillTimer(hwnd,1);
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}