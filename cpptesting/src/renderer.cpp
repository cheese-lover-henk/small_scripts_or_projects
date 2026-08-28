#include "renderer.h"

void Renderer::init() {
    std::cout << "renderer init\n";
}

void Renderer::render(VideoBuffer* buffer) {
    fillRandColor(buffer);
}

void Renderer::fillRandColor(VideoBuffer* buffer) {
    uint64_t seed = rand();
    srand(seed * time(NULL));
    uint32_t i = 0;
    const double PI = 3.141592653589793;
    
    uint32_t w = buffer->width;
    uint32_t h = buffer->height;
    
    uint32_t pitch = buffer->pitch;
    
    uint8_t* first_address_in_row = (uint8_t*)buffer->memory;
    for(uint32_t i = 0; i < h; i++) {
        uint32_t *pixel_address = (uint32_t*) first_address_in_row;
        for(uint32_t j = 0; j < w; j++) {
            
            //color calculation per-pixel
            
            uint32_t value = (((i + (rand() % 0xFFFF)) << 16) + (rand() % 0xFFFF));
            uint32_t color = value* 0.01 * sin((2 * PI) / 10);
            
            
            pixel_address[j] = color;
        }
        first_address_in_row = first_address_in_row + pitch;
    }
}