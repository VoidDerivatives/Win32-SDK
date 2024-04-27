#include<windows.h>
#include"logger.h"
#include<stdio.h>

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")


#if MAIN_LOG
    FILE *fp;
#endif

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
        fp = fopen("log.txt","w");
        if(fp == NULL)
            return -1;
    #endif

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
    #if MAIN_LOG
        fclose(fp);
        fp = NULL;
    #endif

    return (int)msg.wParam;
}

#define MY_ARRAY_SIZE(x) (sizeof(x))/(sizeof(x[0]))

LRESULT CALLBACK WndProc(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    PAINTSTRUCT ps;
    HDC hdc;
    static POINT ptPrevious;
    HPEN hPen;
    POINT pt;

    switch(uiMessage)
    {       
            
    case WM_PAINT:
            hdc  = BeginPaint(hwnd,&ps);

          
            EndPaint(hwnd,&ps);
            break;
    
    case WM_MOUSEMOVE:

            if(wParam & MK_LBUTTON)
            {

                hdc = GetDC(hwnd);
        
                MoveToEx(hdc,ptPrevious.x,ptPrevious.y,NULL);

               	hPen = CreatePen(PS_DASHDOT , 1 , RGB(255,255,255));
                SelectObject(hdc,hPen);
                
                   ptPrevious.x = LOWORD(lParam);
                
                    if(60000 < ptPrevious.x &&  ptPrevious.x <= 65535)
                    {
                        
                        if(ptPrevious.x == 0)
                        {
                            ptPrevious.x = 0;
                           
                             break;
                        }
                        else
                        {
                           
                            ptPrevious.x = -ptPrevious.x;
                            
                         }
                    }
               
                ptPrevious.y = HIWORD(lParam);

               if(60000 < ptPrevious.y &&  ptPrevious.y <= 65535)
                    {
                        
                        if(ptPrevious.y == 0)
                        {
                            ptPrevious.y = 0;
                           
                             break;
                        }
                        else
                        {
                           
                            ptPrevious.y = -ptPrevious.y;
                            
                         }
                    }

                #if MAIN_LOG
                    fprintf(fp,"Value of x : %d\n",ptPrevious.x);
                    fprintf(fp,"Value of y : %d\n",ptPrevious.y);
                #endif

                #if MAIN_LOG
                    fprintf(fp,"Value of x : %d\n",LOWORD(lParam));
                    fprintf(fp,"Value of y : %d\n",HIWORD(lParam));
                #endif

                LineTo(hdc,ptPrevious.x , ptPrevious.y);
              
                DeleteObject(hPen);
              
                ReleaseDC(hwnd,hdc);
            
            }

            break;

    case WM_LBUTTONUP:
            ReleaseCapture();
                        break;

    case WM_LBUTTONDOWN:

            InvalidateRect(hwnd,NULL,TRUE);
            SetCapture(hwnd);
            ptPrevious.x = LOWORD(lParam); 
            ptPrevious.y = HIWORD(lParam); 
         break;

    case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}