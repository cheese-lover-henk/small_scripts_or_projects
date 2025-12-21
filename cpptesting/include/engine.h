#pragma once

#include "types.h"

class Engine {
public:
    
    void setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr);
    void startMainLoop();
    void pollWindowEvents();
    void setTargetFPS(uint32_t target);
private:
    uint32_t targetFPS;
    double targetFrametime_seconds;
    bool running;
    rwQueue<WindowEvent>* windowEventQueue;
    
    void exit();
};