#include "types.h"

#include "window.h"
#include "engine.h"

int main() {
    uint32_t windowWidth = 1920;
    uint32_t windowHeight = 1080;
    
    // start window thread
    
    rwQueue<WindowEvent> eq(100);   //1p1c, window is producer, engine is consumer.
    rwQueue<WindowCommand> cq(100); //1p1c, engine is producer, window is consumer.
    FrameBufferManager v;
    v.init();
    
    std::thread windowThread([&] {
        Window window;
        window.setWindowEventQueuePtr(&eq);
        window.setWindowCommandQueuePtr(&cq);
        window.setFrameBufferManager(&v);
        
        window.create(windowWidth, windowHeight, "Window", false);
        window.startMessageLoop();
    });
    
    std::thread engineThread([&] {
        Engine engine;
        engine.setTargetFPS(60);
        engine.setWindowEventQueuePtr(&eq);
        engine.setWindowCommandQueuePtr(&cq);
        engine.setFrameBufferManager(&v);
        
        engine.startMainLoop();
    });
    
    engineThread.join();
    windowThread.join();
    return 0;
}
