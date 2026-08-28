#pragma once

#include "types.h"

class Renderer {
public:
    void init();
    void render(VideoBuffer* buffer);
private:
    void fillRandColor(VideoBuffer* buffer);
};