#include "window.h"

const char g_szClassName[] = "Window";
bool Window::classRegistered = false;

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam) {
    Window* window = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
    
    
    WindowEvent e;
    switch(msg) {
        // user input events
        case WM_KEYDOWN:
            e.type = KEYDOWN;
            e.pl.key.keycode = (int)wParam;
            window->eventQueue->enqueue(e);
            break;
        case WM_KEYUP:
            e.type = KEYUP;
            e.pl.key.keycode = (int)wParam;
            window->eventQueue->enqueue(e);
            break;
        case WM_MOUSEMOVE:
            e.type = MOUSEMOVE;
            e.pl.mouse.x = (int)(short)LOWORD(lParam);
            e.pl.mouse.y = (int)(short)HIWORD(lParam);
            window->eventQueue->enqueue(e);
            break;
        case WM_LBUTTONDOWN:
            e.type = MOUSEDOWN_L;
            e.pl.mouse.x = (int)(short)LOWORD(lParam);
            e.pl.mouse.y = (int)(short)HIWORD(lParam);
            window->eventQueue->enqueue(e);
            break;
        case WM_LBUTTONUP:
            e.type = MOUSEUP_L;
            e.pl.mouse.x = (int)(short)LOWORD(lParam);
            e.pl.mouse.y = (int)(short)HIWORD(lParam);
            window->eventQueue->enqueue(e);
            break;
        case WM_RBUTTONDOWN:
            e.type = MOUSEDOWN_R;
            e.pl.mouse.x = (int)(short)LOWORD(lParam);
            e.pl.mouse.y = (int)(short)HIWORD(lParam);
            window->eventQueue->enqueue(e);
            break;
        case WM_RBUTTONUP:
            e.type = MOUSEUP_R;
            e.pl.mouse.x = (int)(short)LOWORD(lParam);
            e.pl.mouse.y = (int)(short)HIWORD(lParam);
            window->eventQueue->enqueue(e);
            break;
        
        // window management events
        case WM_CLOSE:
            e.type = CLOSE;
            window->eventQueue->enqueue(e);
            break;
        case WM_DESTROY:
            PostQuitMessage(0);
            break;
        case WM_SIZE:
            e.type = RESIZE;
            e.pl.size.x = LOWORD(lParam);
            e.pl.size.y = HIWORD(lParam);
            window->eventQueue->enqueue(e);
            window->bufferManager->recreateFrameBuffers(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_PAINT: {
            
            VideoBuffer* videobuffer = window->bufferManager->getFrontBuffer();
            
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
        
            StretchDIBits(
                hdc,
                0, 0, videobuffer->width, videobuffer->height,
                0, 0, videobuffer->width, videobuffer->height,
                videobuffer->memory,
                &videobuffer->info,
                DIB_RGB_COLORS,
                SRCCOPY
            );
        
            EndPaint(hwnd, &ps);
            break;
        }
        default:
            return DefWindowProc(hwnd, msg, wParam, lParam);
    }
    return 0;
}

void Window::destroy() {
    PostMessage(hwnd, WM_DESTROY, 0, 0);
    running = false;
}

void Window::setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr) {
    eventQueue = queue_ptr;
}

void Window::setWindowCommandQueuePtr(rwQueue<WindowCommand>* queue_ptr) {
    commandQueue = queue_ptr;
}

void Window::setFrameBufferManager(FrameBufferManager* vbuffermgr) {
    bufferManager = vbuffermgr;
    std::cout << "setframebuffermgr in window\n";
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

    if(!RegisterClassEx(&wc)) {
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

    if(hwnd == NULL) {
        MessageBox(NULL, "Window Creation Failed!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }

    if(maximized) {
        ShowWindow(hwnd, SW_SHOWMAXIMIZED);
    } else {
        ShowWindow(hwnd, SW_SHOWDEFAULT);
    }
    
    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    uint32_t clientWidth  = clientRect.right - clientRect.left;
    uint32_t clientHeight = clientRect.bottom - clientRect.top;

    bufferManager->recreateFrameBuffers(clientWidth, clientHeight);

    UpdateWindow(hwnd);
}

void Window::startMessageLoop() {
    if(eventQueue == NULL) {
        MessageBox(NULL, "Dont start message loop before creating window event queue!", "Error!",
            MB_ICONEXCLAMATION | MB_OK);
        return;
    }
    
    running = true;
    MSG Msg;
    
    while(running) {
        // handle ALL pending windows msgs
        while(PeekMessage(&Msg, NULL, 0, 0, PM_REMOVE) > 0) {
            TranslateMessage(&Msg);
            DispatchMessage(&Msg);
        }
        
        // handle ALL incoming commands from command queue
        WindowCommand c;
        
        while(commandQueue->try_dequeue(c)) {
            switch(c.type) {
                case PAINT_FRAME:
                    InvalidateRect(hwnd, nullptr, FALSE);
                    break;
                case CLOSE_WINDOW:
                    destroy();
                    break;
            }
        }
        Sleep(1);
    }
}

void Window::setTitle(std::string newTitle) {
    if (hwnd) {
        SetWindowText(hwnd, newTitle.c_str());
    }
}