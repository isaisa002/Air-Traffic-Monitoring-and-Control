#ifndef COMPUTER_SYSTEM_H
#define COMPUTER_SYSTEM_H

#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"

class ComputerSystem {
public:
    // Constructor
    ComputerSystem();

    // Function to initialize shared memory for ComputerSystem
    SharedMemoryStruct* initComputerSystemSharedMemory(bool create = false);

    // Function to handle computer system actions (e.g., process tasks)
    void processComputerSystem();

private:
    Logger compSysLog;
};

#endif
