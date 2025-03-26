#include "Radar.h"

// Constructor
Radar::Radar() {
    pthread_mutex_init(&radarMutex, NULL);
}

// Add aircraft to radar
void Radar::addAircraft(Aircraft* aircraft) {
    pthread_mutex_lock(&radarMutex);
    trackedAircraft[aircraft->getID()] = aircraft;
    pthread_mutex_unlock(&radarMutex);
}

// Radar scanning using a POSIX thread
void* Radar::scanAirspace(void* arg) {
    Radar* radar = (Radar*)arg;
    while (true) {
        pthread_mutex_lock(&radar->radarMutex);
        for (const auto& entry : radar->trackedAircraft) {
            cout << "Radar Tracking -> " << entry.second->getRadarInfo() << endl;
        }
        pthread_mutex_unlock(&radar->radarMutex);
        usleep(5000000); // Scan every 5 seconds
    }
    return NULL;
}
