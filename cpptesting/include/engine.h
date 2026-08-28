#pragma once

#include "types.h"
#include "renderer.h"

class Engine {
public:
    FrameBufferManager* bufferManager;
    
    void setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr);
    void setWindowCommandQueuePtr(rwQueue<WindowCommand>* queue_ptr);
    void setFrameBufferManager(FrameBufferManager* vbuffermgr);
    void startMainLoop();
    void pollWindowEvents();
    void setTargetFPS(uint32_t target);
private:
    uint32_t mouseX;
    uint32_t mouseY;
    
    Renderer renderer;
    uint32_t targetFPS;
    double targetFrametime_seconds;
    
    bool running;
    rwQueue<WindowEvent>* windowEventQueue;
    rwQueue<WindowCommand>* commandQueue;
    
    void exit();
};