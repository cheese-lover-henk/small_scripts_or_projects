#include <iostream>
#include <cstdint>
#include <thread>
#include <future>
#include "src/window.h"

int main() {
    
    // start window thread, and retrieve the window handle
    std::promise<HWND> hwndPromise;
    auto hwndFuture = hwndPromise.get_future();
    
    std::thread windowThread([&] { 
        Window window;
        window.create(1920, 1080, "Window", false);
        hwndPromise.set_value(window.hwnd);
        window.startMessageLoop();
    });
    
    HWND window_handle = hwndFuture.get();
    
    // makeshift temporary game loop
    // if clicking X on window, it still waits for this loop to finish. this is unintended but wont be fixed because this is a temporary loop anyways.
    std::string input;
    while(true) {
        std::cin >> input;
        if (input == "exit") {
            break;
        } else {
            InvalidateRect(window_handle, nullptr, FALSE);
        }
    }
    PostMessage(window_handle, WM_DESTROY, 0, 0);
    windowThread.join();
    return 0;
}
