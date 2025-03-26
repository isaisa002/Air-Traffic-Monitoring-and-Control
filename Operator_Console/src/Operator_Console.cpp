#include <iostream>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"


using namespace std;

int main() {
    cout << "[Operator_Console] Hello from Operator_Console!" << endl;

    // Create shared memory (true: create and initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Operator_Console] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Operator_Console] Shared memory opened successfully." << endl;

    // Sleep for 1 second using Timer's static method
    cout << "[Operator_Console] Sleeping for 1 second using Timer..." << endl;
    Timer::sleepSeconds(1);

    // Log a test message using Logger's static method
    Logger::logMessage("[Operator_Console] Test log entry from Operator_Console.");
    cout << "[Operator_Console] Logger wrote to file successfully." << endl;

    // Clean up shared memory when done
    delete shm;
    return 0;
}
