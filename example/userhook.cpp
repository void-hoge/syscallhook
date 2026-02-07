#include "syscallhook.hpp"
#include <cstdio>
#include <string>
#include <map>
#include <chrono>
#include <linux/videodev2.h>

class Timer {
    std::chrono::system_clock::time_point start;
public:
    Timer() : start(std::chrono::system_clock::now()) {}

    uint64_t elapsed_ms() {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now() - this->start).count();
    }
};

static int g_framecount = 0;
static Timer g_timer;

extern "C" void syscallhook_ioctl_post(const sh_ioctl_args* args, int* result, int* errno_inout) {
    if (args->request == VIDIOC_DQBUF && *result == 0) {
        if (g_framecount == 0)
            g_timer = Timer();
        g_framecount++;
    }
}

__attribute__((constructor)) void init() {
    g_framecount = 0;
}

__attribute__((destructor)) void calculate_fps() {
    auto ms = g_timer.elapsed_ms();
    std::printf("fps: %lf\n", (double)g_framecount/ms*1000);
}
