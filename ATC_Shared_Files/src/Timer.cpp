#include "Timer.h"
#include <unistd.h>  // For sleep() in QNX
#include <chrono>    // For chrono and nanosleep
#include <thread>    // To use POSIX nanosleep directly
using namespace std;

void Timer::sleepSeconds(int seconds) {
    sleep(seconds);  // Using POSIX sleep for whole seconds (QNX compatible)
}

void Timer::sleepMilliseconds(int milliseconds) {
    // Using POSIX nanosleep for milliseconds
    struct timespec ts;
    ts.tv_sec = milliseconds / 1000;  // Seconds part
    ts.tv_nsec = (milliseconds % 1000) * 1000000;  // Nanoseconds part
    nanosleep(&ts, NULL);  // Sleep for the specified time
}
