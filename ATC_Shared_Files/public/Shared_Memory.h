#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>
#include <ctime>

// Shared memory name and size constants
#define SHM_NAME "/atc_shm"
#define MAX_AIRCRAFT 100

// Aircraft data structure to hold position, speed, etc.
struct AircraftData {
    int ID;
    float X, Y, Z;
    float SpeedX, SpeedY, SpeedZ;
    time_t timestamp;
    bool active;
};

// Structure for shared memory, containing multiple aircraft data
struct SharedMemoryStruct {
    pthread_mutex_t mutex;                 // Mutex for thread synchronization
    AircraftData aircrafts[MAX_AIRCRAFT];  // Array of aircraft data
    int aircraft_count;                    // Number of active aircraft
    int lookahead_seconds;                 // Time window for checking separation
    bool alert_triggered;                  // Flag to indicate if an alert was triggered
};

// Function to initialize shared memory
SharedMemoryStruct* initSharedMemory(bool create);

#endif
