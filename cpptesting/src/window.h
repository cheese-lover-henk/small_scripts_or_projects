#pragma once

#include <iostream>
#include <cstdint>
#include <vector>
#include <windows.h>

struct WindowCreationInfo {
    uint32_t width;
    uint32_t height;
    std::string title;
    bool maximized;
};

struct videoBuffer {
    BITMAPINFO bitmapInfo;
    void* memory;
    uint32_t width;
    uint32_t height;
    uint32_t pitch = width * 4; // pitch is the size of one row, but because i have no padding or anything, its just (width * size of px), which is 32bits AKA 4 bytes.
};

class Window {
public:
    uint32_t width;
    uint32_t height;

    void createVideoBuffers();
    void recreateVideoBuffers();
    void create(uint32_t w, uint32_t h, std::string title, bool maximized);
    void setTitle(std::string newTitle);
    void startMessageLoop();

private:
    HWND hwnd;
    
    static bool classRegistered;
    static void registerWindowClass(HINSTANCE hInstance);
    
    //std::vector<videoBuffer> bufferQueue; ???
    
    void updateWindowSize(uint32_t w, uint32_t h);
};