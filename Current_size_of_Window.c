#include <Windows.h>

#pragma comment(lib, "user32.lib")
#pragma comment(lib, "kernel32.lib")
#pragma comment(lib, "gdi32.lib")

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nShowCmd){

    static TCHAR szClassName[] = TEXT("The standard window");
    static TCHAR szWindowTitle[] = TEXT("Current Size Of Window");

    HWND hwnd = NULL;

    WNDCLASSEX wnd;
    MSG msg;

    ZeroMemory(&wnd, sizeof(WNDCLASSEX));
    ZeroMemory(&msg, sizeof(MSG));

    wnd.cbSize = sizeof(WNDCLASSEX);
    wnd.cbClsExtra = 0;
    wnd.cbWndExtra = 0;
    wnd.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
    wnd.hCursor = LoadCursor((HINSTANCE)NULL,IDC_ARROW);
    wnd.hIcon = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
    wnd.hIconSm = LoadIcon((HINSTANCE)NULL, IDI_APPLICATION);
    wnd.lpfnWndProc = WndProc;
    wnd.lpszClassName = szClassName;
    wnd.lpszMenuName = NULL;
    wnd.hInstance = hInstance;
    wnd.style = CS_HREDRAW | CS_VREDRAW;

    if(!RegisterClassEx(&wnd)){
        MessageBox(
            NULL,
            TEXT("Failed to register a winodw class"),
            TEXT("RegisterClassEx"),
            MB_TOPMOST | MB_ICONERROR
        );
        ExitProcess(EXIT_FAILURE);
        }


    hwnd = CreateWindowEx(
        WS_EX_APPWINDOW,
        szClassName,
        szWindowTitle,
        WS_OVERLAPPED | WS_SYSMENU | WS_THICKFRAME | WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        (HWND)NULL,
        (HMENU)NULL,
        hInstance,
        (LPVOID)NULL
    );

    if(hwnd == NULL){
        MessageBox(
            NULL,
            TEXT("Failed to create a window class"),
            TEXT("CreateWindowEx"),
            MB_TOPMOST | MB_ICONERROR
        );
        ExitProcess(EXIT_FAILURE);
    }

    ShowWindow(hwnd, nShowCmd);
    UpdateWindow(hwnd);

    while(GetMessage(&msg, NULL, 0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return((int)msg.wParam);

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){

    #define MAX_CHARS  30
    static int cxScreen, cyScreen;
    static TCHAR szOutMsg[MAX_CHARS];
    int iLength;

    PAINTSTRUCT ps;
    HDC hdc = NULL;

    switch(uMsg){
        case WM_PAINT:
            hdc = BeginPaint(hwnd, &ps);
            ZeroMemory(szOutMsg,MAX_CHARS);
            iLength = wsprintf(szOutMsg, TEXT("WIDTH = %d  HEIGHT = %d"), cxScreen, cyScreen);
            SetTextAlign(hdc, TA_TOP | TA_CENTER);

            TextOut(hdc, cxScreen/2, cyScreen/2, szOutMsg, iLength);
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

    return((DefWindowProc(hwnd, uMsg, wParam, lParam)));
}