#pragma once

#include "types.h"

struct WindowCreationInfo {
    uint32_t width;
    uint32_t height;
    std::string title;
    bool maximized;
};

struct VideoBuffer {
    BITMAPINFO info;
    void* memory;
    uint32_t width; //in px
    uint32_t height; // in px
    uint32_t pitch; // pitch is the size of one row, in bytes
};



class Window {
public:
    HWND hwnd;
    rwQueue<WindowEvent>* eventQueue;
    VideoBuffer videobuffer;

    void destroy();
    void triggerBufferDraw();
    void recreateVideoBuffer(uint32_t w, uint32_t h);
    void create(uint32_t w, uint32_t h, std::string title, bool maximized);
    void setTitle(std::string newTitle);
    void setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr);
    void startMessageLoop();
    
    void fillColor(uint32_t color); // color format: 0x00RRGGBB (little-endian BGRA in memory)
    void fillRandColor();
private:
    
    static bool classRegistered;
    static void registerWindowClass(HINSTANCE hInstance);
    
    //std::vector<videoBuffer> bufferQueue; maybe use for double or triple buffering later
};