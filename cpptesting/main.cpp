#include "types.h"

#include "window.h"
#include "engine.h"

int main() {
    
    // start window thread
    
    rwQueue<WindowEvent> q(100);
    
    std::thread windowThread([&] {
        Window window;
        window.create(1920, 1080, "Window", false);
        window.setWindowEventQueuePtr(&q);
        window.startMessageLoop();
    });
    
    std::thread engineThread([&] {
        Engine engine;
        engine.setTargetFPS(240);
        engine.setWindowEventQueuePtr(&q);
        engine.startMainLoop();
    });
    
    engineThread.join();
    windowThread.join();
    return 0;
}
