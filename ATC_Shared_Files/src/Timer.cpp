#include "Timer.h"
#include <time.h>
#include <unistd.h>

// Sleep for the specified number of milliseconds
void sleepForMilliseconds(int ms) {
    timespec req{};
    req.tv_sec = ms / 1000;
    req.tv_nsec = (ms % 1000) * 1000000L;
    nanosleep(&req, nullptr);
}

// Sleep for the specified number of seconds
void sleepForSeconds(int seconds) {
    timespec req{};
    req.tv_sec = seconds;
    req.tv_nsec = 0;
    nanosleep(&req, nullptr);
}
