#include "window.h"

const char g_szClassName[] = "Window";
bool Window::classRegistered = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    switch(msg)
    {
        case WM_CLOSE:
            DestroyWindow(hwnd);
        break;
        case WM_DESTROY:
            PostQuitMessage(0);
        break;
        case WM_SIZE:
            window->width = LOWORD(lParam);
            window->height = HIWORD(lParam);
        break;
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Window::registerWindowClass(HINSTANCE hInstance) {
    WNDCLASSEX wc;

    wc.cbSize        = sizeof(WNDCLASSEX);
    wc.style         = 0;
    wc.lpfnWndProc   = WndProc;
    wc.cbClsExtra    = 0;
    wc.cbWndExtra    = 0;
    wc.hInstance     = hInstance;
    wc.hIcon         = LoadIcon(NULL, IDI_APPLICATION);
    wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = (HBRUSH)(COLOR_WINDOW+1);
    wc.lpszMenuName  = NULL;
    wc.lpszClassName = g_szClassName;
    wc.hIconSm       = LoadIcon(NULL, IDI_APPLICATION);

    if(!RegisterClassEx(&wc))
    {
        MessageBox(NULL, "Window Registration Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    
    classRegistered = true;
}

void Window::create(uint32_t w, uint32_t h, std::string title, bool maximized) {
    HINSTANCE hInstance = GetModuleHandle(NULL);
    
    if(!classRegistered) {
        Window::registerWindowClass(hInstance);
    }
    
    uint32_t spawnPosX = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
    uint32_t spawnPosY = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

    
    hwnd = CreateWindowEx (
        WS_EX_CLIENTEDGE,
        g_szClassName,
        title.c_str(),
        WS_OVERLAPPEDWINDOW,
        spawnPosX, spawnPosY, w, h,
        NULL, NULL, hInstance, NULL
    );

    // associate this window object's pointer with the window created in OS
    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)this);

    if(hwnd == NULL)
    {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    if(maximized) {
        ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    } else {
        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }
    UpdateWindow(hwnd);
    
    Window::width = w;
    Window::height = h;
}

void Window::startMessageLoop() {
    MSG Msg;
    while(GetMessage(&Msg, NULL, 0, 0) > 0) {
        TranslateMessage(&Msg);
        DispatchMessage(&Msg);
    }
}

void Window::setTitle(std::string newTitle) {
    if (hwnd) {
        SetWindowText(hwnd, newTitle.c_str());
    }
}
