#ifndef TIMER_H
#define TIMER_H

#include <chrono>  // For chrono and time-related functions
using namespace std;  // Using std namespace for easier access to standard library features

class Timer {
public:
    // Sleep for a specified number of seconds (QNX compatible)
    static void sleepSeconds(int seconds);

    // Sleep for a specified number of milliseconds (QNX compatible)
    static void sleepMilliseconds(int milliseconds);
};

#endif // TIMER_H
