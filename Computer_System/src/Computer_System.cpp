#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"
int main() {
    std::cout << "[Computer System] Hello from Computer System!" << std::endl;
/*
    // Initialize shared memory (read-only access for test)
    SharedMemoryStruct* shm = initSharedMemory(false);
    if (!shm) {
        std::cerr << "[Computer system] Failed to open shared memory." << std::endl;
        return 1;
    } else {
        std::cout << "[Computer System] Shared memory opened successfully." << std::endl;
    }
*/
    // Use Timer from Timer.h
    std::cout << "[Computer System] Sleeping for 1 second using Timer.h..." << std::endl;
    sleepForSeconds(1);

    // Use Logger to write something to log
    Logger radarLog("comp_sys_log.txt");
    radarLog.log("[Computer System] Test log entry from Radar_Subsystem.");

    std::cout << "[Computer System] Logger wrote to file successfully." << std::endl;

    return 0;
}
