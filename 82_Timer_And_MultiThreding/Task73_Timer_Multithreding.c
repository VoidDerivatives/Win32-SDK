#include<windows.h>
#include"resource.h"
#include<process.h>
#include<tchar.h>

// changing System Menu
#define IDM_SYS_ABOUT   1
#define IDM_SYS_MYCLOSE 2

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
LRESULT CALLBACK WndProc3(HWND,UINT,WPARAM,LPARAM);
LRESULT CALLBACK WndProc4(HWND,UINT,WPARAM,LPARAM);

int cyChar;

// For Timer its 0 and for Thread its 1
BOOL bTimerORThread = 0;

typedef struct
{
    HWND hwnd;
    int cxClient;
    int cyClient;
    int cyChar;
    BOOL bKill;
}PARAMS,*PPARAMS;

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
    HMENU hMenu;

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
                    TEXT("TIMER and MUTITHREDING"),
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

    hMenu = GetSystemMenu(hwnd,FALSE);
    AppendMenu(hMenu,MF_SEPARATOR,0,NULL);
    AppendMenu(hMenu,MF_STRING,IDM_SYS_ABOUT,TEXT("About..."));
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
    static HWND hwndChild[4];
    static HMENU hwndMenu;
    static TCHAR *szChildClass[]={TEXT("Child1"),TEXT("Child2"),TEXT("Child3"),TEXT("Child4")};
    static WNDPROC ChildProc[]={WndProc1,WndProc2,WndProc3,WndProc4};
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

                for(i = 0 ; i < 4 ; i++)
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

            for(i = 0;i < 4 ;i++)
                MoveWindow(hwndChild[i],(i%2)*cxClient/2,(i>1)*cyClient/2,cxClient/2,cyClient/2,TRUE); 

            break;        

        case WM_TIMER:

            for(i = 0;i<4;i++)
                SendMessage(hwndChild[i] , WM_TIMER ,wParam ,lParam);
            break;      

        case WM_CHAR:
         if(wParam == 'x1B')
          {
            DestroyWindow(hwnd);
          }      
            break; 
        
        case WM_SYSCOMMAND:
            switch(LOWORD(wParam))
            {
                case IDM_SYS_ABOUT:
                MessageBox(hwnd,TEXT("Added System Menu") ,TEXT("System Menu"),MB_ICONEXCLAMATION | MB_OK);
                    
                    break;
            

                case IDM_SYS_MYCLOSE:
                    PostQuitMessage(0);
                    break;    
            }      
            break;

        case WM_COMMAND:
           
            switch(LOWORD(wParam))
            {
                    case ID_TIMER_START:

                        bTimerORThread = 0;
                        KillTimer(hwnd,1);
                        SetTimer(hwnd,1,10,NULL);
                        EnableMenuItem(hwndMenu,ID_TIMER_STOP,MF_ENABLED);
                        EnableMenuItem(hwndMenu,ID_TIMER_START,MF_DISABLED);

                        EnableMenuItem(hwndMenu,ID_MULTI_START,MF_DISABLED);
                         break;

                    case ID_TIMER_STOP:
                         KillTimer(hwnd,1);
                         EnableMenuItem(hwndMenu,ID_TIMER_CLEAR,MF_ENABLED);
                         EnableMenuItem(hwndMenu,ID_TIMER_STOP,MF_DISABLED);
                           
                         break;

                    case ID_MULTI_START:

                        bTimerORThread = 1;
                        EnableMenuItem(hwndMenu,ID_MULTI_STOP,MF_ENABLED);
                        EnableMenuItem(hwndMenu,ID_MULTI_START,MF_DISABLED);

                        EnableMenuItem(hwndMenu,ID_TIMER_START,MF_DISABLED);
                        for(i = 0;i<4;i++)
                        {
                            SendMessage(hwndChild[i] , WM_MYCREATE ,wParamForCreate ,lParamForCreate);
                            SendMessage(hwndChild[i] , WM_MYSIZE ,wParamForSize ,lParamForSize);
                        }
                              
                        break;
                                     
                    case ID_MULTI_STOP:
                        
                        EnableMenuItem(hwndMenu,ID_MULTI_CLEAR,MF_ENABLED);
                        EnableMenuItem(hwndMenu,ID_MULTI_STOP,MF_DISABLED);
                      
                        for(i = 0;i<4;i++)
                        {
                            SendMessage(hwndChild[i] , WM_DESTROY ,wParam ,lParam);     
                        }
                    
                        break;

                    case ID_TIMER_CLEAR:

                          EnableMenuItem(hwndMenu,ID_TIMER_START,MF_ENABLED);
                          EnableMenuItem(hwndMenu,ID_TIMER_CLEAR,MF_DISABLED);
                           
                          EnableMenuItem(hwndMenu,ID_MULTI_START,MF_ENABLED);
                    case ID_MULTI_CLEAR:
                        for(i = 0;i<4;i++)
                        {
                            InvalidateRect(hwndChild[i],NULL,TRUE);
                        }
                        if(ID_MULTI_CLEAR)
                        {
                            EnableMenuItem(hwndMenu,ID_TIMER_START,MF_ENABLED);
                            EnableMenuItem(hwndMenu,ID_MULTI_START,MF_ENABLED);
                            EnableMenuItem(hwndMenu,ID_MULTI_CLEAR,MF_DISABLED);
                        }
                        break;  

                    case ID_HELP_HELP:
                            MessageBox(hwnd,TEXT("Help Not yet Implimented!"),TEXT("HELP"),MB_OK);
                            break;  

                    case ID_HELP_ABOUT:
                            MessageBox(hwnd,TEXT("Difference Between Timer And MultiThreding") TEXT("\n\n\t\t\t\t-Aishwarya"),TEXT("ABOUT") , MB_OK);
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

int CheckBottom(HWND hwnd,int cyClient,int iLine)
{
    RECT rect;

    GetClientRect(hwnd,&rect);
    rect.top = 32;
    if(iLine *cyChar+cyChar > cyClient)
    {
        InvalidateRect(hwnd,&rect,TRUE);
        UpdateWindow(hwnd);
        iLine =2;
    }
    return iLine;
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
        iLine =2;
    }
    return iLine;
}

// Increasion sequence of Numbers
void Thred1(PVOID pvoid)
{
    static int iNum = 0,iLine = 2;
    PPARAMS pparams;
    HDC hdc;
    TCHAR szBuffer[16];

    pparams = (PPARAMS)pvoid;

    while(!pparams->bKill)
    {
        if(iNum < 0)
            iNum = 0;

        iLine = CheckBottomMulti(pparams->hwnd,pparams->cyClient,pparams->cyChar,iLine);
        hdc = GetDC(pparams->hwnd);
        TextOut(hdc,0,iLine * pparams->cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum++));
        ReleaseDC(pparams->hwnd,hdc);
            iLine++;
            
    }

    _endthread();
}

LRESULT CALLBACK WndProc1(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static int iNum,iLine=2,cyClient;
    HDC hdc;
    TCHAR szBuffer[16];
    static PARAMS params;

    if(!bTimerORThread)
    {
        switch(uiMessage)
        {
            case WM_SIZE:
                cyClient = HIWORD(lParam);
            break;

            case WM_TIMER:
                if(iNum < 0)
                    iNum = 0;

                iLine = CheckBottom(hwnd,cyClient,iLine);
                hdc = GetDC(hwnd);
                TextOut(hdc,0,iLine * cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum++));
                ReleaseDC(hwnd,hdc);
                iLine++;
                break;

             case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Serial Numbers") ,(int)_tcslen(TEXT("Serial Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;     
        }
    }
    else
    {
        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.bKill= FALSE;
                params.hwnd = hwnd;
                params.cyChar = HIWORD(GetDialogBaseUnits());
                _beginthread(Thred1,0,&params);
            break;

            case WM_MYSIZE:
                params.cyClient = HIWORD(lParam);
                 
                break;
            case WM_DESTROY:
            params.bKill= TRUE;
            break; 

            case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Serial Numbers") ,(int)_tcslen(TEXT("Serial Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;    
        }
    }

    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

// increasing primer numbers
void Thred2(PVOID pvoid)
{
    static int iNum = 1,iLine = 2;
    HDC hdc;
    PPARAMS pparams;
    int i,iSqrt;
    TCHAR szBuffer[16];

    pparams = (PPARAMS) pvoid;

    while(!pparams->bKill)
    {
        do
        {
            if(++iNum < 0)
              iNum = 0;

            iSqrt = (int)sqrt(iNum);
                for(i = 2; i <= iSqrt;i++)
                {
                    if(iNum % i ==  0)
                        break;
                }
        }while(i <= iSqrt);

             iLine = CheckBottomMulti(pparams->hwnd,pparams->cyClient,pparams->cyChar,iLine);
            hdc = GetDC(pparams->hwnd);
            TextOut(hdc,0,iLine * pparams->cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum));
            ReleaseDC(pparams->hwnd,hdc);
            iLine++;
           
    }

    _endthread();
}

LRESULT CALLBACK WndProc2(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static int iNum = 1,iLine=2,cyClient;
    HDC hdc;
    int i,iSqrt;
    TCHAR szBuffer[16];
    static PARAMS params;

    if(!bTimerORThread)
    {
        switch(uiMessage)
        {
            case WM_SIZE:
                cyClient = HIWORD(lParam);
            break;

            case WM_TIMER:
                do
                {
                    if(++iNum < 0)
                        iNum = 0;
                    iSqrt = (int)sqrt(iNum);
                    for(i = 2; i <= iSqrt;i++)
                    {
                        if(iNum % i ==  0)
                            break;
                    }
                }while(i <= iSqrt);

                iLine = CheckBottom(hwnd,cyClient,iLine);
                hdc = GetDC(hwnd);
                TextOut(hdc,0,iLine * cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum));
                ReleaseDC(hwnd,hdc);
                iLine++;
                break;

             case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Prime Numbers") ,(int)_tcslen(TEXT("Prime Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;    
        }
    }
    else
    {
        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.bKill= FALSE;
                params.hwnd = hwnd;
                params.cyChar = HIWORD(GetDialogBaseUnits());
                _beginthread(Thred2,0,&params);
            break;

            case WM_MYSIZE:
                params.cyClient = HIWORD(lParam);
                break;
            case WM_DESTROY:
            params.bKill= TRUE;
            break; 

           case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("Prime Numbers") ,(int)_tcslen(TEXT("Prime Numbers")));
                ReleaseDC(hwnd,hdc);  
                break;     
        }
    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

//Finonacci number
void Thred3(PVOID pvoid)
{
     static int iNum = 0,iNext = 1,iLine = 2,iTemp;
    HDC hdc;
    PPARAMS pparams;
    TCHAR szBuffer[16];

    pparams = (PPARAMS) pvoid;

    while(!pparams->bKill)
    {
        if(iNum < 0)
            {
                iNum = 0;
                iNext = 1;
            }

            iLine = CheckBottomMulti(pparams->hwnd,pparams->cyClient,pparams->cyChar,iLine);
            hdc = GetDC(pparams->hwnd);
            TextOut(hdc,0,iLine * pparams->cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum++));
            ReleaseDC(pparams->hwnd,hdc);
            iTemp = iNum;
            iNum = iNext;
            iNext += iTemp;
            iLine++;
           
    }

    _endthread();
}

LRESULT CALLBACK WndProc3(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static int iNum = 0,iNext = 1,iLine=2,cyClient;
    HDC hdc;
    int iTemp;
    TCHAR szBuffer[16];
    static PARAMS params;

    if(!bTimerORThread)
    {
        switch(uiMessage)
        {
            case WM_SIZE:
                cyClient = HIWORD(lParam);

            break;

            case WM_TIMER:
                if(iNum < 0)
                {
                    iNum = 0;
                    iNext = 1;
                }

                iLine = CheckBottom(hwnd,cyClient,iLine);
                hdc = GetDC(hwnd);
                TextOut(hdc,0,iLine * cyChar , szBuffer , wsprintf(szBuffer,TEXT("%d"),iNum++));
                ReleaseDC(hwnd,hdc);
                iTemp = iNum;
                iNum = iNext;
                iNext += iTemp;
                iLine++;
                break;

            case WM_PAINT:

                hdc = GetDC(hwnd);
                TextOut(hdc,20, 0 , TEXT("fibonacci Series") ,(int)_tcslen(TEXT("fibonacci Series")));
                ReleaseDC(hwnd,hdc);  
                break;     
        }
    }
    else
    {
        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.bKill= FALSE;
                params.hwnd = hwnd;
                params.cyChar = HIWORD(GetDialogBaseUnits());
                _beginthread(Thred3,0,&params);
            break;

            case WM_MYSIZE:
                params.cyClient = HIWORD(lParam);
                break;
            case WM_DESTROY:
            params.bKill= TRUE;
            break; 

            case WM_PAINT:

                hdc = GetDC(hwnd);
               TextOut(hdc,20, 0 , TEXT("fibonacci Series") ,(int)_tcslen(TEXT("fibonacci Series")));
               ReleaseDC(hwnd,hdc);  
                break;    
        }
    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}

void Thred4(PVOID pvoid)
{
    PPARAMS pparams;
    HDC hdc;
    int iDiameter;

    pparams = (PPARAMS) pvoid;

    while(!pparams->bKill)
    {
            InvalidateRect(pparams->hwnd,NULL,TRUE);
            UpdateWindow(pparams->hwnd);
            iDiameter = rand()%(max(1,min(pparams->cxClient,pparams->cyClient)));
            hdc = GetDC(pparams->hwnd);

            Ellipse(hdc,(pparams->cxClient - iDiameter) /2,
                        (pparams->cyClient - iDiameter) / 2,
                        (pparams->cxClient + iDiameter) /2,
                        (pparams->cyClient + iDiameter) /2
                        );
            ReleaseDC(pparams->hwnd,hdc);
           
    }

    _endthread();
}
LRESULT CALLBACK WndProc4(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    static int cxClient,cyClient;
    HDC hdc;
    int iDiameter;
    static PARAMS params;

    if(!bTimerORThread)
    {
        switch(uiMessage)
        {
            case WM_SIZE:
                cyClient = HIWORD(lParam);
                cxClient = LOWORD(lParam);

            break;

            case WM_TIMER:
                InvalidateRect(hwnd,NULL,TRUE);
                UpdateWindow(hwnd);
                iDiameter = rand()%(max(1,min(cxClient,cyClient)));
                hdc = GetDC(hwnd);

                Ellipse(hdc,(cxClient - iDiameter) /2,
                            (cyClient - iDiameter) / 2,
                            (cxClient + iDiameter) /2,
                            (cyClient + iDiameter) /2
                            );
                ReleaseDC(hwnd,hdc);
                
                break;
        }
    }
    else
    {
        switch(uiMessage)
        {
            case WM_MYCREATE:
                params.bKill= FALSE;
                params.hwnd = hwnd;
               params.cyChar = HIWORD(GetDialogBaseUnits());
                _beginthread(Thred4,0,&params);
            break;

            case WM_MYSIZE:
                params.cyClient = cyClient;
                params.cxClient = cxClient;
                break; 
            case WM_DESTROY:
                params.bKill= TRUE;
            break;   
        }
    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}