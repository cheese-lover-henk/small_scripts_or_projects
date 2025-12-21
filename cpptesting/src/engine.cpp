#include "engine.h"


void Engine::startMainLoop() {
    running = true;
    
    // all durations in seconds.
    auto lastTime = Clock::now();
    double delta;
    
    //for fps counting
    uint32_t frames = 0;
    double seconds_passed = 0.0;
    
    // init renderer
    Renderer renderer;
    renderer.init();
    while(running) {
        // for the loop timings
        auto frameStartTime = Clock::now();
        delta = std::chrono::duration<double>(frameStartTime - lastTime).count();
        lastTime = frameStartTime;
        // for fps counting
        seconds_passed += delta;
        frames++;
        if(seconds_passed > 1.0) {
            std::cout << "FPS: " << frames << " (/" << targetFPS << ")\n";
            frames = 0;
            seconds_passed -= 1.0;
        }
        
        bufferManager->swapBuffers();
        
        
        
        pollWindowEvents();
        //update game state
        renderer.render(bufferManager->getBackBuffer());
        
        //tell Window class to display the rendered image
        WindowCommand c;
        c.type = PAINT_FRAME;
        commandQueue->enqueue(c);
        
        
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
            case MOUSEMOVE:
                mouseX = e.pl.mouse.x;
                mouseY = e.pl.mouse.y;
                break;
            case MOUSEDOWN_L:
                mouseX = e.pl.mouse.x;
                mouseY = e.pl.mouse.y;
                std::cout << "mouse clicked at: (" << mouseX << ", " << mouseY << ")\n";
                break;
            case RESIZE:
                bufferManager->recreateFrameBuffers(e.pl.size.x, e.pl.size.y);
                break;
            case CLOSE:
                WindowCommand c;
                c.type = CLOSE_WINDOW;
                commandQueue->enqueue(c); //send window the command to close
                
                exit(); //close ourselves (engine)
                break;
        }
        succeeded = windowEventQueue->try_dequeue(e);
    }
}

void Engine::setTargetFPS(uint32_t target) {
    targetFPS = target;
    targetFrametime_seconds = 1.0 / target;
    std::cout << "set target fps to: " << target << "\n";
}

void Engine::exit() {
    running = false;
}

void Engine::setWindowEventQueuePtr(rwQueue<WindowEvent>* queue_ptr) {
    windowEventQueue = queue_ptr;
}

void Engine::setWindowCommandQueuePtr(rwQueue<WindowCommand>* queue_ptr) {
    commandQueue = queue_ptr;
}

void Engine::setFrameBufferManager(FrameBufferManager* vbuffermgr) {
    bufferManager = vbuffermgr;
    std::cout << "setframebuffermgr in engine\n";
}