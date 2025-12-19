#include <iostream>
#include <vector>
#include <cstdint>


struct test {
    uint32_t field;
};


int main() {
    uint8_t c = 255;
    std::cout << static_cast<unsigned>(c) << ", " << sizeof(c) << "\n";
    return 0;
}
