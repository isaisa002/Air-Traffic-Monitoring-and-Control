#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"
using namespace std;

int main() {
    cout << "[Radar] Hello from Radar_Subsystem!" << endl;

    // Create shared memory (true: create and initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Radar] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Radar] Shared memory opened successfully." << endl;

    // Sleep for 1 second using Timer's static method
    cout << "[Radar] Sleeping for 1 second using Timer..." << endl;
    Timer::sleepSeconds(1);

    // Log a test message using Logger's static method
    Logger::logMessage("[Radar] Test log entry from Radar_Subsystem.");
    cout << "[Radar] Logger wrote to file successfully." << endl;

    // Clean up shared memory when done
    delete shm;
    return 0;
}
