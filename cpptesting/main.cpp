#include <iostream>
#include <cstdint>
#include <thread>
#include "src/window.h"


int main() {
    
    std::thread windowThread([] {
        Window window;
        window.create(800, 500, "Window", false);
        window.startMessageLoop();
    });
    
    windowThread.join();
    return 0;
}
