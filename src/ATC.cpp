#include <iostream>
#include <pthread.h>
#include "Aircraft.h"
#include "Radar.h"

using namespace std;

int main() {
    // Create aircraft
    Aircraft aircraft1(1, 10000, 20000, 15000, 300, 200, 0);
    Aircraft aircraft2(2, 12000, 22000, 16000, 250, 180, 0);

    // Create radar system
    Radar radar;
    radar.addAircraft(&aircraft1);
    radar.addAircraft(&aircraft2);

    // Start aircraft movement threads
    pthread_t aircraftThread1, aircraftThread2, radarThread;
    pthread_create(&aircraftThread1, NULL, Aircraft::startSimulation, &aircraft1);
    pthread_create(&aircraftThread2, NULL, Aircraft::startSimulation, &aircraft2);
    pthread_create(&radarThread, NULL, Radar::scanAirspace, &radar);

    // Run for 10 seconds
    sleep(10);

    // Display final report
    cout << "\nFinal Radar Report:\n";
    cout << aircraft1.getRadarInfo() << endl;
    cout << aircraft2.getRadarInfo() << endl;

    // Change altitude
    aircraft1.processCommand("change_altitude", 17000);
    cout << "\nAfter altitude change for Aircraft 1:\n";
    cout << aircraft1.getRadarInfo() << endl;

    // Terminate
    pthread_cancel(aircraftThread1);
    pthread_cancel(aircraftThread2);
    pthread_cancel(radarThread);

    cout << "\nATC System Test Completed (QNX).\n";
    return 0;
}
