#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"


using namespace std;

int main() {
    cout << "[Computer_System] Hello from Computer_System!" << endl;

    // Create shared memory (true: create and initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Computer_System] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Computer_System] Shared memory opened successfully." << endl;

    // Sleep for 1 second using Timer's static method
    cout << "[Computer_System] Sleeping for 1 second using Timer..." << endl;
    Timer::sleepSeconds(1);

    // Log a test message using Logger's static method
    Logger::logMessage("[Computer_System] Test log entry from Computer_System.");
    cout << "[Computer_System] Logger wrote to file successfully." << endl;

    // Clean up shared memory when done
    delete shm;
    return 0;
}
