#include "engine.h"


void Engine::startMainLoop() {
    running = true;
    
    // all durations in seconds.
    auto lastTime = Clock::now();
    double delta;
    
    //for fps counting
    uint32_t frames = 0;
    double seconds_passed = 0.0;
    
    
    while(running) {
        // for the loop timings
        auto frameStartTime = Clock::now();
        delta = std::chrono::duration<double>(frameStartTime - lastTime).count();
        lastTime = frameStartTime;
        // for fps counting
        seconds_passed += delta;
        frames++;
        if(seconds_passed > 1.0) {
            std::cout << "FPS: " << frames << "\n";
            frames = 0;
            seconds_passed -= 1.0;
        }
        
        
        
        pollWindowEvents();
        //update game state
        //render game
        //tell Window class to display the rendered image
        
        
        // if there is time left, then just do nothing until the target frame time (matching target fps) is reached
        auto nextFrame = frameStartTime + std::chrono::duration<double>(targetFrametime_seconds);
        while (Clock::now() < nextFrame) {
            // wait
        }
    }
}

void Engine::pollWindowEvents() {
    WindowEvent e;
    bool succeeded = windowEventQueue->try_dequeue(e);
    
    while(succeeded) {
        switch(e.type) {
            case CLOSE:
                exit();
                break;
            case KEYDOWN: {
                int code = e.pl.key.keycode;
                std::cout << "keydown: " << (char)code << "\n";
                
                break;
            }
            case KEYUP: {
                int code = e.pl.key.keycode;
                std::cout << "keyup: " << (char)code << "\n";
                
                break;
            }
            default:
                int i = 0;
        }
        
        succeeded = windowEventQueue->try_dequeue(e);
    }
}

void Engine::setTargetFPS(uint32_t target) {
    targetFPS = target;
    targetFrametime_seconds = 1.0 / target;
}

void Engine::exit() {
    running = false;
}

void Engine::setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr) {
    windowEventQueue = queue_ptr;
}