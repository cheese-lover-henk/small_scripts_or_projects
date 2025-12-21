#pragma once

#include "types.h"

class Window {
public:
    HWND hwnd;
    rwQueue<WindowEvent>* eventQueue;
    rwQueue<WindowCommand>* commandQueue;
    FrameBufferManager* bufferManager;

    void destroy();
    void triggerBufferDraw();
    void create(uint32_t w, uint32_t h, std::string title, bool maximized);
    void setTitle(std::string newTitle);
    void setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr);
    void setWindowCommandQueuePtr(rwQueue<WindowCommand>* queue_ptr);
    void setFrameBufferManager(FrameBufferManager* vbuffermgr);
    void startMessageLoop();
private:
    bool running;
    
    static bool classRegistered;
    static void registerWindowClass(HINSTANCE hInstance);
};