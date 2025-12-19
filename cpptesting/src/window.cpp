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
            window->recreateVideoBuffer(LOWORD(lParam), HIWORD(lParam));
            break;
        case WM_PAINT: {
            window->fillRandColor();
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
        
            StretchDIBits(
                hdc,
                0, 0, window->videobuffer.width, window->videobuffer.height,
                0, 0, window->videobuffer.width, window->videobuffer.height,
                window->videobuffer.memory,
                &window->videobuffer.info,
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
    Window::recreateVideoBuffer(w, h);
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

void Window::recreateVideoBuffer(uint32_t width, uint32_t height) { //width and height in bytes
    if (videobuffer.memory)
        VirtualFree(videobuffer.memory, 0, MEM_RELEASE);
    videobuffer.width = width;
    videobuffer.height = height;
    
    uint32_t alignment = 32; //bytes alignment.
    videobuffer.pitch = width * 4; //row size in bytes, each px color is 4 bytes (a uint32) and 1 "width" is 1 color px, so the total bytes in the row is 4bytes x (amount of pixels) width
    //videobuffer.pitch = (rowSize + alignment - 1) & ~(alignment - 1); //this sets the pitch to the nearest number divisible by 32, higher than current width


    videobuffer.info = {};
    videobuffer.info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    videobuffer.info.bmiHeader.biWidth = width;
    videobuffer.info.bmiHeader.biHeight = -height; // top-down
    videobuffer.info.bmiHeader.biPlanes = 1;
    videobuffer.info.bmiHeader.biBitCount = 32;
    videobuffer.info.bmiHeader.biCompression = BI_RGB;
    
    
    uint32_t buffersize = videobuffer.pitch * videobuffer.height;
    videobuffer.memory = VirtualAlloc(
        0,
        buffersize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    std::cout << "recreated video buffer: " << width << "x"<< height << " (" << buffersize << ") bytes, /32 =" << (buffersize / 32) << "\n"; // \32 to check if alignment works
}

void Window::fillColor(uint32_t color) {
    printf("%02X \n", color);
    
    uint32_t w = videobuffer.width;
    uint32_t h = videobuffer.height;
    
    uint32_t pitch = videobuffer.pitch;
    
    uint8_t* first_address_in_row = (uint8_t*)videobuffer.memory;
    for(uint32_t i = 0; i < h; i++) {
        uint32_t *pixel_address = (uint32_t*) first_address_in_row;
        for(uint32_t j = 0; j < w; j++) {
            pixel_address[j] = color;
        }
        first_address_in_row = first_address_in_row + pitch;
    }
}

void Window::fillRandColor() {
    std::string hex = (std::stringstream() << std::hex << std::setw(6) << std::setfill('0') << (rand() & 0xFFFFFF)).str();
    Window::fillColor(std::stoul(hex, nullptr, 16));
}