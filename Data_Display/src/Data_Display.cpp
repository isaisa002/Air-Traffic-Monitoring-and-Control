#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"


using namespace std;

int main() {
    cout << "[Data_Display] Hello from Data_Display!" << endl;

    // Create shared memory (true: create and initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Data_Display] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Data_Display] Shared memory opened successfully." << endl;

    // Sleep for 1 second using Timer's static method
    cout << "[Data_Display] Sleeping for 1 second using Timer..." << endl;
    Timer::sleepSeconds(1);

    // Log a test message using Logger's static method
    Logger::logMessage("[Data_Display] Test log entry from Data_Display.");
    cout << "[Data_Display] Logger wrote to file successfully." << endl;

    // Clean up shared memory when done
    delete shm;
    return 0;
}
