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

// Structure to hold necessary data for each aircraft task
struct AircraftTaskData {
    AircraftManager* manager;
    int index;
    AircraftData aircraft;

    // Constructor to initialize the task data
    AircraftTaskData(AircraftManager* m, int i, const AircraftData& a)
        : manager(m), index(i), aircraft(a) {}
};

class AircraftManager {
public:
    // Constructor: takes the path to the input file and a pointer to shared memory
    AircraftManager(const string inputFilePath, SharedMemory* sharedMemory);
    ~AircraftManager();

    // Start method: reads the input file and creates aircraft tasks
    void start();

    // Command processing functions:
    // Update the speed of an aircraft with the specified ID
    void updateSpeed(int aircraftId, double newSpeedX, double newSpeedY, double newSpeedZ);
    // Update the position of an aircraft with the specified ID
    void updatePosition(int aircraftId, double newPosX, double newPosY, double newPosZ);

    // Function to process commands from shared memory
    void processCommand();

private:
    // The aircraft task: each aircraft runs this periodic function in its thread.
    void aircraftTask(int index, const AircraftData &initialData);
    // Thread wrapper to pass arguments to the aircraft task function.
    static void* aircraftTaskWrapper(void* arg);

    string inputFilePath_;              // Path to the input file with aircraft details.
    SharedMemory* shm_;                 // Pointer to the shared memory for communication.
    vector<pthread_t> aircraftThreads_; // Vector to store thread IDs of aircraft tasks.
};

#endif // AIRCRAFT_MANAGER_H
