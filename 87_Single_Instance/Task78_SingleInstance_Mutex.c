#include<windows.h>
#include"resource.h"
#include<process.h>
#include<tchar.h>

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

#define GCL_HBRBACKGROUND (-10) // Winuser.h
#define GWL_HINSTANCE     (-4)

#define WM_MYCREATE (WM_USER + 0x0001)
#define WM_MYSIZE (WM_USER + 0x0002)


LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

LRESULT CALLBACK WndProc1(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WndProc2(HWND,UINT,WPARAM,LPARAM);

HANDLE hMutex;
int cyChar;
int iNum;

typedef struct
{
    HWND hwnd;
    int cxClient;
    int cyClient;
    int cyChar;
}PARAMS,*PPARAMS;

// for Critical section
HANDLE Array[2];
#define MAX 10000
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
    
    hMutex = CreateMutex(NULL,FALSE,TEXT("My Application"));
    if(ERROR_ALREADY_EXISTS == GetLastError())
    {
        MessageBox(NULL,TEXT("ALREADY RUNNING INSTANCE"),TEXT("WARNING"),MB_OK | MB_ICONASTERISK);
        CloseHandle(hMutex);
        return 0;
    }
    wndclass.cbSize = sizeof(wndclass);
    wndclass.hInstance = hInstance;
    wndclass.lpfnWndProc = WndProc;
    wndclass.lpszClassName = szAppName;
    wndclass.style = CS_HREDRAW | CS_VREDRAW;
    wndclass.hIcon = LoadIcon(hInstance,MAKEINTRESOURCE(IDI_ICON1));
    wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
    wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
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
                    TEXT("ONE INSTANCE RUNNING"),
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
    static HWND hwndChild[4];
    static HMENU hwndMenu;
    static TCHAR *szChildClass[]={TEXT("Child1"),TEXT("Child2")};
    static WNDPROC ChildProc[]={WndProc1,WndProc2};
    HINSTANCE hInstance;
    int i,cxClient , cyClient;
    WNDCLASS wndclass;
    static WPARAM wParamForCreate;
    static LPARAM lParamForCreate;
    static WPARAM wParamForSize;
    static LPARAM lParamForSize;

    switch(uiMessage)
    {     
        case WM_CREATE:
            wParamForCreate = wParam;
            lParamForCreate = lParam;
            hwndMenu = GetMenu(hwnd);

            hInstance = (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE);
                wndclass.hInstance = hInstance;
                wndclass.style = CS_HREDRAW | CS_VREDRAW;
                wndclass.hIcon = NULL;
                wndclass.hCursor = LoadCursor(NULL,IDC_ARROW);
                wndclass.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
                wndclass.lpszMenuName = NULL;
                wndclass.cbClsExtra = 0;
                wndclass.cbWndExtra = 0;

                for(i = 0 ; i < 2 ; i++)
                {
                    wndclass.lpfnWndProc = ChildProc[i];
                    wndclass.lpszClassName = szChildClass[i];   

                    RegisterClass(&wndclass);
                    hwndChild[i] =  CreateWindow(
                                    szChildClass[i],
                                    NULL,
                                    WS_CHILDWINDOW | WS_BORDER | WS_VISIBLE,
                                    0,
                                    0,
                                    0,
                                    0,
                                    hwnd,
                                    (HMENU)i,
                                    hInstance,
                                    NULL
                                    );
                }

                cyChar = HIWORD(GetDialogBaseUnits());
               
                break;

        case WM_SIZE:

            wParamForSize = wParam;
            lParamForSize = lParam;

            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);

            for(i = 0;i < 2 ;i++)
            {
                MoveWindow(hwndChild[i],(i%2)*cxClient/2,(i>1)*cyClient/2,cxClient/2+cxClient/2*i,cyClient,TRUE); 
            }


            break;              

        case WM_CHAR:
         if(wParam == 'x1B')
          {
            DestroyWindow(hwnd);
          }      
            break;    
        case WM_COMMAND:
           
            switch(LOWORD(wParam))
            {
        
                    case ID_MUTEX_START:

                        EnableMenuItem(hwndMenu,ID_MUTEX_START,MF_DISABLED);
                        EnableMenuItem(hwndMenu,ID_MUTEX_CLEAR,MF_ENABLED);

                        for(i = 0;i<2;i++)
                        {
                            SendMessage(hwndChild[i] , WM_MYCREATE ,wParamForCreate ,lParamForCreate);
                            SendMessage(hwndChild[i] , WM_MYSIZE ,wParamForSize ,lParamForSize);
                        }

                        break;
                                     
                    case ID_MUTEX_CLEAR:
                        iNum = 0;
                        for(i = 0;i<2;i++)
                        {
                            InvalidateRect(hwndChild[i],NULL,TRUE);
                        }
                        
                            EnableMenuItem(hwndMenu,ID_MUTEX_START,MF_ENABLED);
                            EnableMenuItem(hwndMenu,ID_MUTEX_CLEAR,MF_DISABLED);
                        
                        break;  

                    case ID_HELP_HELP:
                            MessageBox(hwnd,TEXT("Help Not yet Implimented!"),TEXT("HELP"),MB_OK);
                            break;  

                    case ID_HELP_ABOUT:
                            MessageBox(hwnd,TEXT("Use of Mutex For Single Instance\n") TEXT("\n\n\t\t\t\t-Aishwarya"),TEXT("ABOUT") , MB_OK);
                            break;
            }
            break;
         
        
        case WM_DESTROY:
                KillTimer(hwnd,1);
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

int CheckBottomMulti(HWND hwnd,int cyClient,int cyChar,int iLine)
{
    RECT rect;

    GetClientRect(hwnd,&rect);
    rect.top = 32;
    if(iLine *cyChar+cyChar > cyClient)
    {
        InvalidateRect(hwnd,&rect,TRUE);
        UpdateWindow(hwnd);
        iLine = 2;
    }
    return iLine;
}

// Even Numbers
void Thread1(PVOID pvoid)
{
    static int iLine = 2;
    PPARAMS pparams;
    HDC hdc;
    TCHAR szBuffer[16];

    pparams = (PPARAMS)pvoid;


    while(iNum <= MAX)
    {   
        WaitForSingleObject(hMutex,INFINITE);
        if((iNum % 2) ==0)
        {      

            iLine = CheckBottomMulti(pparams->hwnd,pparams->cyClient,pparams->cyChar,iLine);
            hdc = GetDC(pparams->hwnd);
            TextOut(hdc,15,iLine * pparams->cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum));
            ReleaseDC(pparams->hwnd,hdc);
            iNum++;
            iLine++;
        }
        ReleaseMutex(hMutex);   

    }

    _endthread();
}

LRESULT CALLBACK WndProc1(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static PARAMS params;   
    static uintptr_t hSeq;
    HDC hdc;

        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.hwnd = hwnd;
                params.cyChar = HIWORD(GetDialogBaseUnits());
                hSeq = _beginthread(Thread1,0,&params);
                Array[0] = (HANDLE)hSeq;
            break;

            case WM_MYSIZE:
                params.cyClient = HIWORD(lParam);
                 
                break;
            case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Even Numbers") ,(int)_tcslen(TEXT("Even Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;       
        }
    
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

// Odd numbers
void Thread2(PVOID pvoid)
{
    static int iLine = 2;
    HDC hdc;
    PPARAMS pparams;
    int i,iSqrt;
    TCHAR szBuffer[16];


    pparams = (PPARAMS) pvoid;

    while(iNum <= MAX)
    {
             WaitForSingleObject(hMutex,INFINITE);
            if((iNum % 2) != 0)        
            {   
                iLine = CheckBottomMulti(pparams->hwnd,pparams->cyClient,pparams->cyChar,iLine);
                hdc = GetDC(pparams->hwnd);
                TextOut(hdc,15,iLine * pparams->cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum));
                ReleaseDC(pparams->hwnd,hdc);
                iLine++;
                iNum++;
            }
           ReleaseMutex(hMutex);  
           
    }

    _endthread();
}

LRESULT CALLBACK WndProc2(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static PARAMS params;
    HDC hdc;
    static uintptr_t hPrime;


        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.hwnd = hwnd;
                params.cyChar = HIWORD(GetDialogBaseUnits());
                hPrime = _beginthread(Thread2,0,&params);
                Array[1] = (HANDLE)hPrime;
            break;

            case WM_MYSIZE:
                params.cyClient = HIWORD(lParam);
                break;

            case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Odd Numbers") ,(int)_tcslen(TEXT("Odd Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;          
        }
    
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}