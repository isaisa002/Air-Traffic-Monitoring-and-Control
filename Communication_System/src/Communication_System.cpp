#include "Communication_System.h"
#include "Logger.h"
#include <pthread.h>
#include <iostream>

using namespace std;

CommunicationSystem::CommunicationSystem(SharedMemory* sharedMemory) : shm_(sharedMemory) {}

void CommunicationSystem::sendCommand(int aircraftID, const string& command) {
    // Get shared data
    SharedData* data = shm_->getData();

    // Lock the specific aircraft's data
    pthread_mutex_lock(&data->aircraftLocks[aircraftID]);

    // Simulate sending a command to an aircraft by logging the message
    data->lastOperatorCommand[0] = '\0';  // Clear previous command
    strncpy(data->lastOperatorCommand, command.c_str(), sizeof(data->lastOperatorCommand) - 1);
    Logger::logMessage("[CommunicationSystem] Sent command to Aircraft " + to_string(aircraftID) + ": " + command);

    // Unlock the data for the aircraft
    pthread_mutex_unlock(&data->aircraftLocks[aircraftID]);
}

void CommunicationSystem::receiveData(int aircraftID) {
    SharedData* data = shm_->getData();

    // Lock the aircraft data to read it
    pthread_mutex_lock(&data->aircraftLocks[aircraftID]);

    // Simulate receiving data from the aircraft (e.g., position, speed)
    Logger::logMessage("[CommunicationSystem] Aircraft " + to_string(aircraftID) + " Data: "
                       + " Position(" + to_string(data->aircraftList[aircraftID].posX) + ", "
                       + to_string(data->aircraftList[aircraftID].posY) + ", "
                       + to_string(data->aircraftList[aircraftID].posZ) + ") "
                       + " Speed(" + to_string(data->aircraftList[aircraftID].speedX) + ", "
                       + to_string(data->aircraftList[aircraftID].speedY) + ", "
                       + to_string(data->aircraftList[aircraftID].speedZ) + ")");

    // Unlock the aircraft data
    pthread_mutex_unlock(&data->aircraftLocks[aircraftID]);
}
