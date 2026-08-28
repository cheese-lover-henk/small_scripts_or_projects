#pragma once

#include "types.h"

struct VideoBuffer {
    BITMAPINFO info;
    void* memory;
    uint32_t width; //in px
    uint32_t height; // in px
    uint32_t pitch; // pitch is the size of one row, in bytes
};

class FrameBufferManager {
public:
    VideoBuffer* getBackBuffer(); // for rendering
    VideoBuffer* getFrontBuffer(); // for drawing
    
    void swapBuffers();
    
    void init();
    void recreateFrameBuffers(uint32_t width, uint32_t height);

private:
    VideoBuffer bufferA;
    VideoBuffer bufferB;
    
    std::atomic<VideoBuffer*> front;
    VideoBuffer* back;
    
    void recreateFrameBuffer(VideoBuffer& buffer, uint32_t width, uint32_t height);
};
