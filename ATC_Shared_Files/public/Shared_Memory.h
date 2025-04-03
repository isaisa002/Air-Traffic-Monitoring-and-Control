#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>

#define MAX_AIRCRAFT 10

struct AircraftData {
    int id;
    double posX;
    double posY;
    double posZ;
    double speedX;
    double speedY;
    double speedZ;
    bool isActive;
};

struct SharedData {
    AircraftData aircraftList[MAX_AIRCRAFT];
    char lastOperatorCommand[256];
    int predictionLookahead; // Dynamic prediction lookahead (in seconds)
    pthread_mutex_t globalLock;
    pthread_mutex_t aircraftLocks[MAX_AIRCRAFT];
};

class SharedMemory {
public:
    // Constructor: create or attach to shared memory.
    SharedMemory(bool createNew = true);
    ~SharedMemory();

    SharedData* getData();

    // Locking helpers
    void lockGlobal();
    void unlockGlobal();
    void lockAircraft(int index);
    void unlockAircraft(int index);

private:
    SharedData* data_;
};

#endif // SHARED_MEMORY_H
