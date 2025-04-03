#include "Aircraft_Manager.h"
#include "Logger.h"
#include "Timer.h"
#include "Shared_Memory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <pthread.h>
#include <cstring>
#include <sstream>

using namespace std;

void* AircraftManager::aircraftTaskWrapper(void* arg) {
    auto* data = static_cast<AircraftTaskData*>(arg);
    AircraftManager* manager = data->manager;
    int index = data->index;
    AircraftData initialData = data->aircraft;
    manager->aircraftTask(index, initialData);
    delete data;
    return nullptr;
}

void AircraftManager::aircraftTask(int index, const AircraftData &initialData) {
    SharedData* sharedData = shm_->getData();
    auto taskStart = chrono::steady_clock::now();

    // Initialize aircraft data in shared memory.
    shm_->lockAircraft(index);
    sharedData->aircraftList[index] = initialData;
    sharedData->aircraftList[index].isActive = true;
    shm_->unlockAircraft(index);

    Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) +
                       " started at index " + to_string(index), "AircraftManager");

    while (true) {
        auto startUpdate = chrono::steady_clock::now();

        // Update aircraft position based on its current speed.
        shm_->lockAircraft(index);
        sharedData->aircraftList[index].posX += sharedData->aircraftList[index].speedX;
        sharedData->aircraftList[index].posY += sharedData->aircraftList[index].speedY;
        sharedData->aircraftList[index].posZ += sharedData->aircraftList[index].speedZ;
        shm_->unlockAircraft(index);

        // Check for new commands intended for this aircraft.
        {
            shm_->lockGlobal();
            std::string command(sharedData->lastOperatorCommand);
            shm_->unlockGlobal();

            // Expected format: "aircraftID:COMMAND parameters"
            // e.g., "2:SPEED 0 200 0" or "1:ALTITUDE 18000"
            std::stringstream ss(command);
            int targetID;
            if (ss >> targetID) {
                if (targetID == initialData.id) {
                    ss.ignore(1, ':'); // Skip the colon.
                    std::string action;
                    ss >> action;
                    if (action == "SPEED") {
                        double newSpeedX, newSpeedY, newSpeedZ;
                        ss >> newSpeedX >> newSpeedY >> newSpeedZ;
                        shm_->lockAircraft(index);
                        sharedData->aircraftList[index].speedX = newSpeedX;
                        sharedData->aircraftList[index].speedY = newSpeedY;
                        sharedData->aircraftList[index].speedZ = newSpeedZ;
                        shm_->unlockAircraft(index);
                        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) + " speed updated.");
                    }
                    else if (action == "ALTITUDE") {
                        double newAltitude;
                        ss >> newAltitude;
                        shm_->lockAircraft(index);
                        sharedData->aircraftList[index].posZ = newAltitude;
                        shm_->unlockAircraft(index);
                        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) + " altitude updated.");
                    }
                }
            }
        }

        // Sleep for 1 second before next update.
        Timer::sleepSeconds(1);
    }
}
