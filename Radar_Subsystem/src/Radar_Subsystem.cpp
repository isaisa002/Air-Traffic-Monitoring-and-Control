#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"

int main() {
    std::cout << "[Radar] Hello from Radar_Subsystem!" << std::endl;

    // Initialize shared memory (read-only access for test)
    SharedMemoryStruct* shm = initSharedMemory(false);
    if (!shm) {
        std::cerr << "[Radar] Failed to open shared memory." << std::endl;
        return 1;
    } else {
        std::cout << "[Radar] Shared memory opened successfully." << std::endl;
    }

    // Use Timer from Timer.h
    std::cout << "[Radar] Sleeping for 1 second using Timer.h..." << std::endl;
    sleepForSeconds(1);

    // Use Logger to write something to log
    Logger radarLog("radar_log.txt");
    radarLog.log("[Radar] Test log entry from Radar_Subsystem.");

    std::cout << "[Radar] Logger wrote to file successfully." << std::endl;

    return 0;
}
