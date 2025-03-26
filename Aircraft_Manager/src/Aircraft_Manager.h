
#ifndef AIRCRAFT_MANAGER_H
#define AIRCRAFT_MANAGER_H

#include "Shared_Memory.h"
#include <string>
#include <vector>
#include <pthread.h>
#include <chrono>
using namespace std;

// Forward declaration of AircraftManager class
class AircraftManager;

// Struct to hold the necessary data for the aircraft task
struct AircraftTaskData {
    AircraftManager* manager;
    int index;
    AircraftData aircraft;

    // Constructor to initialize the data
    AircraftTaskData(AircraftManager* m, int i, const AircraftData& a)
        : manager(m), index(i), aircraft(a) {}
};

class AircraftManager {
public:
    AircraftManager(const string inputFilePath, SharedMemory* sharedMemory);
    ~AircraftManager();

    // Starts the aircraft tasks
    void start();

private:
    // Task for each aircraft (periodic)
    void aircraftTask(int index, const AircraftData &initialData);

    // Wrapper for threading (to pass arguments to member functions)
    static void* aircraftTaskWrapper(void* arg);

    string inputFilePath_;  // Path to input file containing aircraft information
    SharedMemory* shm_;  // Shared memory for communication
    vector<pthread_t> aircraftThreads_;  // List of threads (tasks) for each aircraft
};

#endif // AIRCRAFT_MANAGER_H




/*
#ifndef RADAR_H
#define RADAR_H

#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"



#endif
*/
