#include<windows.h>
#include<windowsx.h>
#include<process.h>
#include<tchar.h>
#include<stdio.h>
#include"Task80_Doubly_circular_List.h"

#define WINDOW_WIDTH    800
#define WINDOW_HEIGHT   600

#pragma comment(lib,"user32.lib")
#pragma comment(lib,"kernel32.lib")
#pragma comment(lib,"gdi32.lib")

// Macros ID
#define INSERT_LIST          0
#define DELETE_LIST          1
#define SEARCH_LIST          2
#define REVERSE_LIST         3
#define CONCATE_LIST         4
#define COUNT_LIST           5
#define COMBO_BOX            6
#define BUTTON_SELECTION     7
#define EDIT_NUMBER          8
#define DISPLAY              9
#define DO_BUTTON            10

#define GCL_HBRBACKGROUND (-10) // Winuser.h
#define GWL_HINSTANCE     (-4)

LRESULT CALLBACK WndProc(HWND,UINT,WPARAM,LPARAM);

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
                    TEXT("Doubly Circular LinkedList"),
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
void InsertFirst(struct node **head,struct node **tail,int no)
{
    FILE *fp = NULL;
    struct node *newnode = NULL;
    fp = fopen("demo.txt","a");

    if(fp == NULL)
    {
            return ;
    }

    //fprintf(fp, "\nIn InsertFirst\n");
    //fflush(fp);

    newnode = (struct node *)malloc(sizeof(struct node));

    if(newnode == NULL)
    {
        //printf("Memory allocation failed");
        return ;
    }
    newnode -> data = no;

    //fprintf(fp, "newnode -> data :  %d\n",newnode -> data);
    //fflush(fp);

    if(*head == NULL)
    {
        //fprintf(fp, "Enter If *head == NULL\n");
        //fflush(fp);
        *head = newnode;
        *tail = newnode;
        //fprintf(fp, "End If *head == NULL\n");
        //fprintf(fp, "newnode :\t%d\n",newnode);
        //fflush(fp);
    }
    else
    {

        //fprintf(fp,"Enter NOT *head == NULL\n");
        ////fprintf(fp, "*head :\t%d",*head);
        //fflush(fp);
        newnode -> next = *head;

        //fprintf(fp,"Befor (*head) -> prev = newnode\n");
        ////fprintf(fp, "*head->prev :\t%d",(*head)->prev);
        
        //fflush(fp);
        
        (*head) -> prev = newnode;
        fprintf(fp, "After *head->prev :\t%d\n",(*head)->prev);
        fclose(fp);
        
        //fprintf(fp,"After (*head = newnode)\n");
        //fflush(fp);
        
        *head = newnode; 
        
        //fprintf(fp,"End NOT *head == NULL\n");
        //fflush(fp);
    }

    //fprintf(fp, "After IF\n");
    //fflush(fp);
    
    (*tail) -> next = *head;   //circular effect
    (*head) -> prev = *tail;   // circular effect
    newnode = NULL;

    //fprintf(fp, "End InsertFirst");
    //fflush(fp);
    //fclose(fp);
    fp = NULL;

}

void displayUI(struct node *head,struct node *tail,int *data)
{
    FILE *fp = NULL;
    int iCount = 0;
    if(head == NULL)
    {
        //printf("\nList is empty\n");
        return ;
    }
    
    fp = fopen("demo.txt","a");
        if(fp == NULL)
        {
            return ;
        }
    do
    {
        
        data[iCount] = head -> data;
        fprintf(fp, "data[iCount] : \t%d\n", data[iCount]);
        fprintf(fp, "head -> data : \t%d\n", head -> data);
        //fflush(fp);
        //head -> data;
        head = head -> next;
        
        iCount++;
    }while(head != tail -> next);

    fclose(fp);
   fp =NULL;
    
}
FILE *fp = NULL;
LRESULT CALLBACK WndProc(HWND hwnd,
                        UINT uiMessage,
                        WPARAM wParam,
                        LPARAM lParam)
{
    RECT rect;
    PAINTSTRUCT ps;
    HDC hdc;
    HBRUSH hBrush;
    int iCounter;

    int iBufferSize=0;
    static TCHAR Buffer[50];

    static int cxClient;
    static int cyClient;

    static HWND hwndButton[6];
    static HWND hwndComboBox;
    static HWND hwndButtonS;
    static HWND hwndEditBoxNumber;
    static HWND hwndEditBoxPosition;
    static HWND hwndDisplay;
    static HWND hwndDo;
    static TCHAR *cArr[6] = {TEXT("INSERT"),TEXT("DELETE"),TEXT("SEARCH"),TEXT("REVERSE"),TEXT("CONCATE"),TEXT("COUNT")};
    static TCHAR *cArrInsert[3] = {TEXT("INSERT FIRST"),TEXT("INSERT LAST"),TEXT("INSERT AT POSITION")};
    static TCHAR *cArrDelete[3] = {TEXT("DELETE FIRST"),TEXT("DELETE LAST"),TEXT("DELETE AT POSITION")};
    static TCHAR *cArrSearch[3] = {TEXT("SEARCH FIRST"),TEXT("SEARCH LAST"),TEXT("SEARCH AT POSITION")};
    static TCHAR *cArrReverse[2] = {TEXT("PHYSICAL REVERSE"),TEXT("REVERSE DISPLAY")};
    static TCHAR *cArrConcate[2] = {TEXT("CONCATE LIST"),TEXT("CONCATE AT POSITION")};
       
    static HMODULE hModule = NULL; 
    
    static struct node *first = NULL;
    static struct node *last = NULL;
    static struct node *second = NULL;
    static struct node *last1 = NULL;

    int data=0;
    TCHAR szText[255]; 
    int iLength,number;    
    static int arrData[1000];

    switch(uiMessage)
    {  
        case WM_CREATE:
            for(iCounter = 0;iCounter<6;iCounter++)
            {
                hwndButton[iCounter] =  CreateWindow(
                        TEXT("button"),
                        NULL,
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)iCounter,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );

            }

            hwndComboBox =  CreateWindow(
                        TEXT("COMBOBOX"),
                        NULL,
                        CBS_DROPDOWN | CBS_HASSTRINGS | WS_CHILD | WS_OVERLAPPED | WS_VISIBLE,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)COMBO_BOX,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );

            hwndButtonS =  CreateWindow(
                        TEXT("button"),
                        NULL,
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)BUTTON_SELECTION,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );    

            hwndEditBoxNumber = CreateWindow(
                        TEXT("Edit"),
                        NULL,
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY  | ES_LOWERCASE 
                        | SS_BLACKFRAME  | WS_BORDER | ES_AUTOVSCROLL ,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)EDIT_NUMBER,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );

            
            hwndDisplay =  CreateWindow(
                        TEXT("button"),
                        NULL,
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)DISPLAY,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );

            hwndDo =  CreateWindow(
                        TEXT("button"),
                        NULL,
                        WS_TABSTOP | WS_VISIBLE | WS_CHILD | BS_NOTIFY,
                        0,
                        0,
                        0,
                        0,
                        hwnd,
                        (HMENU)DO_BUTTON,
                        (HINSTANCE)GetWindowLong(hwnd,GWL_HINSTANCE),
                        NULL
                        );
            break;

        case WM_SIZE:

            cxClient = LOWORD(lParam);
            cyClient = HIWORD(lParam);
                
            // Button    
            for(iCounter=0;iCounter<6;iCounter++)
            {
                MoveWindow(hwndButton[iCounter],10,cyClient-cyClient/2+100+iCounter*25+10,100,20,TRUE);
                ShowWindow(hwndButton[iCounter],SW_SHOWNORMAL);
                SetWindowText(hwndButton[iCounter],cArr[iCounter]);
            }

            SetWindowText(hwndButtonS,TEXT(">>"));

            MoveWindow(hwndDisplay,cxClient-60,cyClient-30,50,20,TRUE);
            ShowWindow(hwndDisplay,SW_SHOWNORMAL);
            SetWindowText(hwndDisplay,TEXT("SHOW"));

            SetWindowText(hwndDo,TEXT("DO"));


         break;    

        case WM_PAINT:

                hdc = BeginPaint(hwnd,&ps);
                
                GetClientRect(hwnd,&rect);

                // All Area
            
                hBrush = CreateSolidBrush(RGB(135,176,230));
                SelectObject(hdc,hBrush);
                Rectangle(hdc,5,5,cxClient - 5 ,cyClient - 5);
                DeleteObject(hBrush);

                MoveToEx(hdc,0,cyClient-cyClient/2+100,NULL);    
                LineTo(hdc,cxClient,cyClient-cyClient/2+100);

                EndPaint(hwnd,&ps);
           
                break;  

        case WM_COMMAND:

                hdc = GetDC(hwnd);
                switch(HIWORD(wParam))
                {
                    case BN_CLICKED:

                   
                        if(LOWORD(wParam) == DO_BUTTON)
                        {
                            //fp = fopen("demo.txt","a");
                            memset(szText,255, 255*sizeof(TCHAR));
                            iLength = SendMessage(hwndEditBoxNumber,EM_GETLINE,0,(LPARAM)szText);
                            szText[iLength]='\0';
                            sscanf(szText, "%d", &number);

                            // load DLL
                            //fprintf(fp, "Before LoadLibrary \n");
                            //fflush(fp);
                           /* hModule = LoadLibrary("Task80_Doubly_Circular_LinkedList.dll");
                            if(hModule == NULL)
                            {
                                MessageBox(hwnd,TEXT("LoadLibrary : Failure"),TEXT("DLL"),MB_OK);
                                return -1;
                            }
                            fprintf(fp, "After LoadLibrary \n");
                            fflush(fp);

                            if(!strcmp(Buffer,cArrInsert[0])) 
                            {

                                
                                fPtrInsertFAndL = (FPTRINSERTFL)GetProcAddress(hModule,"InsertFirst");

                                if(NULL == fPtrInsertFAndL)
                                {
                                    MessageBox(hwnd,TEXT("GetProcAddress : Failure"),TEXT("DLL"),MB_OK);
                                    return -1;
                                }
                                
                                fprintf(fp, "Befor InsertFirst\n");
                                fprintf(fp, "The number is :\t%d\n", number);
                                fflush(fp);
                                fclose(fp);
                                fp = NULL;
                                fPtrInsertFAndL(&first,&last,number);
                                fp = fopen("demo.txt","a");

                                fprintf(fp, "After InsertFirst\n");
                               fprintf(fp, "The number is :\t%d\n", number);
                               fflush(fp);
                               
                                fPtrInsertFAndL = NULL;
                            }

                            fptrDisplayUI = (FPTRDISPLAYUI)GetProcAddress(hModule,"displayUI");

                            if(NULL == fptrDisplayUI)
                            {
                                MessageBox(hwnd,TEXT("GetProcAddress : Failure"),TEXT("DLL"),MB_OK);
                                
                                return -1;
                            }
                            
                            fprintf(fp, "Befor displayUI\n");
                            fflush(fp);
                            
                            fptrDisplayUI(first,last,arrData);
                            fptrDisplayUI = NULL;
                            
                            fprintf(fp, "After displayUI\n");
                            fflush(fp);

                            /*for(iCounter = 0;iCounter <5;iCounter++)
                            {
                                fprintf(fp, "%d", arrData[iCounter]);
                                iCounter++;
                            }
                            fclose(fp);
                            fp = NULL;*/

                            InsertFirst(&first,&last,number);
                            displayUI(first,last,arrData);
                             //fclose(fp);
                            //fp = NULL;
                            // Free DLL
                            /*if(hModule)
                            {
                                FreeLibrary(hModule);
                                hModule = NULL;
                            }
                            fprintf(fp, "-----------------------------------\n\n");
                            fflush(fp);

                            fclose(fp);
                            fp = NULL;*/
                        }
                        else if(LOWORD(wParam) != BUTTON_SELECTION)
                        {
                            ShowWindow(hwndComboBox,SW_HIDE);   
                            ShowWindow(hwndButtonS,SW_HIDE);                 
                            ComboBox_ResetContent(hwndComboBox);
                                    
                                if(LOWORD(wParam) == INSERT_LIST)    // INSERT
                                {
                                   
                                    MoveWindow(hwndComboBox,150,cyClient-cyClient/2+110,200,150,TRUE);
                                    
                                    // Button
                                    MoveWindow(hwndButtonS,cxClient/2-30,cyClient-cyClient/2+110,20,25,TRUE);
                                     
                                    for(iCounter=0;iCounter<3;iCounter++)
                                    {
                                        ComboBox_AddItemData(hwndComboBox,cArrInsert[iCounter]);
                                    } 
                                }

                                if(LOWORD(wParam) == DELETE_LIST)    // delete
                                {
                                    
                                    MoveWindow(hwndComboBox,150,cyClient-cyClient/2+110+25,200,150,TRUE);
                                   
                                   // Button
                                    MoveWindow(hwndButtonS,cxClient/2-30,cyClient-cyClient/2+110+25,20,25,TRUE);
                                     
                                    for(iCounter=0;iCounter<3;iCounter++)
                                    {
                                        ComboBox_AddItemData(hwndComboBox,cArrDelete[iCounter]);
                                    } 
                                }

                                if(LOWORD(wParam) == SEARCH_LIST)    // search
                                {
                                    MoveWindow(hwndComboBox,150,cyClient-cyClient/2+110+50,200,150,TRUE);
                                    
                                    // Button
                                    MoveWindow(hwndButtonS,cxClient/2-30,cyClient-cyClient/2+110+50,20,25,TRUE);
                                     
                                    for(iCounter=0;iCounter<3;iCounter++)
                                    {
                                        ComboBox_AddItemData(hwndComboBox,cArrSearch[iCounter]);
                                    } 
                                }

                                if(LOWORD(wParam) == REVERSE_LIST)    // Reverse
                                {
                                   
                                    MoveWindow(hwndComboBox,150,cyClient-cyClient/2+110+75,200,150,TRUE);
                                    // Button
                                    MoveWindow(hwndButtonS,cxClient/2-30,cyClient-cyClient/2+110+75,20,25,TRUE);
                                     
                                    for(iCounter=0;iCounter<3;iCounter++)
                                    {
                                        ComboBox_AddItemData(hwndComboBox,cArrReverse[iCounter]);
                                    } 
                                }

                                if(LOWORD(wParam) == CONCATE_LIST)    // Reverse
                                {
                                   
                                    MoveWindow(hwndComboBox,150,cyClient-cyClient/2+110+100,200,150,TRUE);
                                    // Button
                                    MoveWindow(hwndButtonS,cxClient/2-30,cyClient-cyClient/2+110+100,20,25,TRUE);
                                     
                                    for(iCounter=0;iCounter<3;iCounter++)
                                    {
                                        ComboBox_AddItemData(hwndComboBox,cArrConcate[iCounter]);
                                    } 
                                }
                                ShowWindow(hwndComboBox,SW_SHOWNORMAL);
                                ShowWindow(hwndButtonS,SW_SHOWNORMAL);
                                if(LOWORD(wParam) == COUNT_LIST)    // Reverse
                                {
                                     ShowWindow(hwndComboBox,SW_HIDE);
                                     ShowWindow(hwndButtonS,SW_HIDE);             
                                }
                        }
                        else if(LOWORD(wParam) == BUTTON_SELECTION)
                        {
                            memset(Buffer,0, 50*sizeof(TCHAR));
                            ComboBox_GetText(hwndComboBox,Buffer,sizeof(Buffer));
                            
                            if(!strcmp(Buffer,cArrInsert[0]) || !strcmp(Buffer,cArrInsert[1]) || !strcmp(Buffer,cArrInsert[2]))
                            {
                                MoveWindow(hwndEditBoxNumber,cxClient/2,cyClient-cyClient/2+110,40,25,TRUE);
                                ShowWindow(hwndEditBoxNumber,SW_SHOWNORMAL);

                                MoveWindow(hwndDo,cxClient/2+50,cyClient-cyClient/2+110,30,25,TRUE);
                                ShowWindow(hwndDo,SW_SHOWNORMAL);  
                            }  
                        }
                    break;
               }

                ReleaseDC(hwnd,hdc);
            
                break;        
        case WM_DESTROY:
                PostQuitMessage(0);
            break;

    }
    return DefWindowProc(hwnd,uiMessage,wParam,lParam);
}


