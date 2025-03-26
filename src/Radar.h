#ifndef RADAR_H
#define RADAR_H

#include <iostream>
#include <map>
#include <pthread.h>
#include "Aircraft.h"

using namespace std;

class Radar {
private:
    map<int, Aircraft*> trackedAircraft;
    pthread_mutex_t radarMutex;

public:
    Radar();
    void addAircraft(Aircraft* aircraft);
    static void* scanAirspace(void* arg);
};

#endif // RADAR_H
