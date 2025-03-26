#ifndef RADAR_H
#define RADAR_H

#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"

class Radar {
public:
    // Constructor
    Radar();

    // Function to initialize shared memory for Radar
    SharedMemoryStruct* initRadarSharedMemory(bool create = false);

    // Function to handle radar actions (e.g., start, stop, etc.)
    void processRadar();

private:
    Logger radarLog;
};

#endif
