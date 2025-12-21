#include <iostream>
#include <cstdint>
#include <vector>
#include <windows.h>
#include <iomanip>
#include <cmath>
#include <chrono>
#include <thread>
#include <future>

#include "readerwriterqueue.h"
#include "windowEvents.h"
#include "framebuffermanager.h"

template<typename T>
using rwQueue = moodycamel::ReaderWriterQueue<T>;

using Clock = std::chrono::steady_clock;