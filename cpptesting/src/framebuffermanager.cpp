#include "framebuffermanager.h"


void FrameBufferManager::init() {
    front.store(&bufferA, std::memory_order_release);
    back = &bufferB;
    std::cout << "buffermgr init\n";
}

VideoBuffer* FrameBufferManager::getBackBuffer() {
    return back;
}

VideoBuffer* FrameBufferManager::getFrontBuffer() {
    return front.load(std::memory_order_acquire);
}

void FrameBufferManager::swapBuffers() {
    VideoBuffer* temp = back;
    back = front.load(std::memory_order_acquire);
    front.store(temp, std::memory_order_release);
}

void FrameBufferManager::recreateFrameBuffers(uint32_t width, uint32_t height) { //width and height in px
    recreateFrameBuffer(bufferA, width, height);
    recreateFrameBuffer(bufferB, width, height);
}

void FrameBufferManager::recreateFrameBuffer(VideoBuffer& buffer, uint32_t width, uint32_t height) {
    if (buffer.memory) {
        VirtualFree(buffer.memory, 0, MEM_RELEASE);
        buffer.memory = nullptr;
    }
    buffer.width = width;
    buffer.height = height;
    //row size in bytes, each px color is 4 bytes (a uint32) and 1 "width" is 1 color px, so the total bytes in the row is 4bytes x (amount of pixels) width
    buffer.pitch = width * 4;


    buffer.info = {};
    buffer.info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    buffer.info.bmiHeader.biWidth = width;
    buffer.info.bmiHeader.biHeight = -static_cast<int32_t>(height); // top-down
    buffer.info.bmiHeader.biPlanes = 1;
    buffer.info.bmiHeader.biBitCount = 32;
    buffer.info.bmiHeader.biCompression = BI_RGB;
    
    
    uint64_t buffersize = uint64_t(buffer.pitch) * buffer.height; //64 incase it gets very big for some reason
    
    buffer.memory = VirtualAlloc(
        0,
        buffersize,
        MEM_COMMIT | MEM_RESERVE,
        PAGE_READWRITE
    );
    
    if (!buffer.memory) {
        std::cerr << "VirtualAlloc failed\n";
        return;
    }
    std::cout << "recreated video buffer: " << width << "x"<< height << " (" << buffersize << ") bytes\n";
}