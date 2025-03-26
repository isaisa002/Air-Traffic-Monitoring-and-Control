#ifndef AIRCRAFT_H
#define AIRCRAFT_H

#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <cstring>

#define SHARED_MEMORY_NAME "/aircraft_shm"

using namespace std;

class Aircraft {
private:
    int id;
    double x, y, z;
    double speedX, speedY, speedZ;
    pthread_mutex_t mtx;   // Mutex for thread safety

public:
    Aircraft(int id, double x, double y, double z, double speedX, double speedY, double speedZ);

    int getID() const;

    void updateLocation();

    string getRadarInfo();

    void processCommand(string command, double value);

    static void* startSimulation(void* arg);

    void logStatus();
};

#endif // AIRCRAFT_H
