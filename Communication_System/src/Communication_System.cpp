#include "Communication_System.h"
#include "Shared_Memory.h"
#include "Logger.h"
#include <pthread.h>
#include <iostream>
#include <cstring>

using namespace std;

CommunicationSystem::CommunicationSystem(SharedMemory* sharedMemory) : shm_(sharedMemory) {}

void CommunicationSystem::sendCommand(int aircraftID, const string& command) {
    SharedData* data = shm_->getData();
    pthread_mutex_lock(&data->aircraftLocks[aircraftID]);
    data->lastOperatorCommand[0] = '\0';  // Clear previous command.
    strncpy(data->lastOperatorCommand, command.c_str(), sizeof(data->lastOperatorCommand) - 1);
    Logger::logMessage("[CommunicationSystem] Sent command to Aircraft " + to_string(aircraftID) + ": " + command);
    pthread_mutex_unlock(&data->aircraftLocks[aircraftID]);
}

void CommunicationSystem::receiveData(int aircraftID) {
    SharedData* data = shm_->getData();
    pthread_mutex_lock(&data->aircraftLocks[aircraftID]);
    Logger::logMessage("[CommunicationSystem] Aircraft " + to_string(aircraftID) + " Data: " +
                       " Position(" + to_string(data->aircraftList[aircraftID].posX) + ", " +
                       to_string(data->aircraftList[aircraftID].posY) + ", " +
                       to_string(data->aircraftList[aircraftID].posZ) + ") " +
                       " Speed(" + to_string(data->aircraftList[aircraftID].speedX) + ", " +
                       to_string(data->aircraftList[aircraftID].speedY) + ", " +
                       to_string(data->aircraftList[aircraftID].speedZ) + ")");
    pthread_mutex_unlock(&data->aircraftLocks[aircraftID]);
}

int main() {
    Logger::logMessage("[CommSystem] Starting Communication System...");
    SharedMemory* shm = new SharedMemory(false);
    CommunicationSystem commSystem(shm);
    
    // Example loop to demonstrate receiving data every 5 seconds.
    while (true) {
        commSystem.receiveData(0);
        Timer::sleepSeconds(5);
    }
    
    delete shm;
    return 0;
}