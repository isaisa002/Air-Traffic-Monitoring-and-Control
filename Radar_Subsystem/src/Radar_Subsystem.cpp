#include "Shared_Memory.h"
#include "Logger.h"
#include "Timer.h"
#include <iostream>
#include <sstream>
#include <cmath>
using namespace std;

int main() {
    Logger::logMessage("[RadarSystem] Starting radar subsystem...");

    // Open existing shared memory (do not create a new one)
    SharedMemory* shm = new SharedMemory(false);

    // Define radar parameters:
    // Assume radar center is at (0,0,0)
    double radarCenterX = 0.0;
    double radarCenterY = 0.0;
    double radarCenterZ = 0.0;
    // Define the illumination radius for PSR simulation (e.g., 50,000 meters)
    double illuminationRadius = 50000.0;

    while (true) {
        SharedData* sharedData = shm->getData();
        stringstream ss;
        ss << "[RadarSystem] Airspace Plan View (PSR/SSR simulation):\n";

        // Loop through each aircraft in shared memory.
        for (int i = 0; i < MAX_AIRCRAFT; ++i) {
            // Acquire the lock for this aircraft before reading its data.
            shm->lockAircraft(i);
            if (sharedData->aircraftList[i].isActive) {
                // Compute the distance from the radar center.
                double dx = sharedData->aircraftList[i].posX - radarCenterX;
                double dy = sharedData->aircraftList[i].posY - radarCenterY;
                double dz = sharedData->aircraftList[i].posZ - radarCenterZ;
                double distance = sqrt(dx * dx + dy * dy + dz * dz);

                // PSR: Only "illuminate" aircraft within the specified radius.
                if (distance <= illuminationRadius) {
                    // SSR simulation: Interrogate the aircraft.
                    Logger::logMessage("[RadarSystem] Interrogating aircraft " + to_string(sharedData->aircraftList[i].id));

                    // Append the aircraft's details (flight ID, flight level, speed, position) to the output.
                    ss << "Aircraft " << sharedData->aircraftList[i].id
                       << " : FL " << sharedData->aircraftList[i].flightLevel
                       << ", Speed (" << sharedData->aircraftList[i].speedX << ", "
                       << sharedData->aircraftList[i].speedY << ", "
                       << sharedData->aircraftList[i].speedZ << ")"
                       << ", Position (" << sharedData->aircraftList[i].posX << ", "
                       << sharedData->aircraftList[i].posY << ", "
                       << sharedData->aircraftList[i].posZ << ")"
                       << ", Distance " << distance << "\n";
                } else {
                    ss << "Aircraft " << sharedData->aircraftList[i].id
                       << " is outside illumination range (distance: " << distance << ")\n";
                }
            }
            // Release the lock for this aircraft.
            shm->unlockAircraft(i);
        }

        // Log the entire plan view.
        Logger::logMessage(ss.str());

        // Wait for 5 seconds before the next radar scan.
        Timer::sleepSeconds(5);
    }

    return 0;
}
