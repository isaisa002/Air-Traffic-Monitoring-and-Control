#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include <pthread.h>
#include <chrono>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
using namespace std;

constexpr int MAX_AIRCRAFT = 50;

struct AircraftData {
    int id;
    bool isActive;
    double posX, posY, posZ;
    double speedX, speedY, speedZ;
    double flightLevel;
    bool hasResponded;

    // Temporal Parameters
    chrono::seconds releaseTime;
    chrono::seconds relativeDeadline;
    chrono::seconds executionTime;
    chrono::seconds deadline;
    chrono::seconds responseTime;
    chrono::seconds period;
};

struct SharedData {
    AircraftData aircraftList[MAX_AIRCRAFT];
    pthread_mutex_t aircraftLocks[MAX_AIRCRAFT];
    int lookaheadSeconds;
    bool alarmTriggered;
    char lastOperatorCommand[256];
    pthread_mutex_t globalLock;
};

class SharedMemory {
public:
    SharedMemory(bool createFlag);
    ~SharedMemory();
    SharedData* getData();

private:
    SharedData* data_;
    int fd_;
    bool owner_;
};

#endif
