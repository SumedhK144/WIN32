#include <Windows.h>

#define NR_ROWS     100
#define NR_COLUMN   100
#define MAX_CHARS   30

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
{
    static TCHAR szWindowClass[] = TEXT("The standard class");
    static TCHAR szWindowTitle[] = TEXT("TEXTOUT");

    HWND hwnd = NULL;
    WNDCLASSEX wnd;
    MSG msg;

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;

    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor(NULL, IDC_ARROW);
    wnd.hIcon = LoadIcon(NULL, IDI_APPLICATION);
    wnd.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
    wnd.hInstance = hInstance;
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szWindowClass;
    wnd.lpszMenuName = NULL;
    wnd.style = CS_HREDRAW | CS_VREDRAW;

    if(!RegisterClassEx(&wnd))
    {
        MessageBox(
            NULL,
            TEXT("Failed to register a Winodw Class"),
            TEXT("RegisterClassEx"),
            MB_ICONERROR | MB_TOPMOST
        );
        ExitProcess(EXIT_FAILURE);
    }

    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szWindowClass,
        szWindowTitle,
        WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInstance,
        NULL
        );

        if(hwnd == NULL)
        {
            MessageBox(
                NULL,
                TEXT("Failed to create a Winodw Class"),
                TEXT("CreateWinodwEx"),
                MB_TOPMOST | MB_ICONERROR
            );
            ExitProcess(EXIT_FAILURE);
        }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return(msg.wParam);



}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam ,LPARAM lParam)
{
    static int cxChar, cyChar;                              
    static int cxScreen, cyScreen;                          
    static TCHAR szOut[MAX_CHARS];
    static TCHAR szMsg[] = TEXT("ROW:%d, COLUMN:%d");

    int iLength, iRowCounter, iColumnCounter;

    PAINTSTRUCT ps;
    TEXTMETRIC tm;
    HDC hdc = NULL;

    switch(uMsg)
    {
        case WM_CREATE:
            hdc = GetDC(hwnd);
            GetTextMetrics(hdc, &tm);
            ReleaseDC(hwnd, hdc);
            cxChar = tm.tmAveCharWidth;
            cyChar = tm.tmHeight + tm.tmExternalLeading;

            break;

        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            for(iRowCounter = 0; iRowCounter < NR_ROWS; ++iRowCounter)
            {
                for(iColumnCounter = 0; iColumnCounter < NR_COLUMN; ++iColumnCounter)
                {
                    ZeroMemory(szOut, MAX_CHARS);
                    iLength = wsprintf(szOut, szMsg, iRowCounter, iColumnCounter);
                    TextOut(
                        hdc,
                        MAX_CHARS* iColumnCounter * cxChar,
                        iRowCounter * cyChar,
                        szOut,
                        iLength
                    );

                }
            }
            EndPaint(hwnd, &ps);
            break;

        case WM_SIZE:
            cxScreen = LOWORD(lParam);
            cyScreen = HIWORD(lParam);
            break;

       

        case WM_DESTROY:
            PostQuitMessage(EXIT_SUCCESS);
            break;

    }

    return(DefWindowProc(hwnd, uMsg, wParam, lParam));
}