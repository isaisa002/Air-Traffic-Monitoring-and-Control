#include "Communication_System.h"
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

int main() {
    Logger::logMessage("[CommSystem] Starting Communication System...");

    // Open existing shared memory (do not create a new one)
    SharedMemory* shm = new SharedMemory(false);
    SharedData* data = shm->getData();

    while (true) {
        // Use the global lock to access the command field safely.
        shm->lockGlobal();

        // Check if there's a new command in lastOperatorCommand field.
        if (strlen(data->lastOperatorCommand) > 0) {
            // Copy the command to a local string for processing.
            string commandStr(data->lastOperatorCommand);
            Logger::logMessage("[CommSystem] Detected command: " + commandStr);

            // Parse the command expecting format "aircraftId:command"
            size_t pos = commandStr.find(':');
            if (pos != string::npos) {
                int aircraftId = stoi(commandStr.substr(0, pos));
                string cmd = commandStr.substr(pos + 1);
                Logger::logMessage("[CommSystem] Sending command '" + cmd + "' to aircraft " + to_string(aircraftId));

                // (Optional) Here you could simulate updating the aircraft's data
                // based on the command, e.g., change speed or position.
            } else {
                Logger::logMessage("[CommSystem] Command format error: " + commandStr);
            }

            // Clear the command field to indicate the command has been processed.
            memset(data->lastOperatorCommand, 0, sizeof(data->lastOperatorCommand));
        }
        shm->unlockGlobal();

        // Check for new commands every second.
        Timer::sleepSeconds(1);
    }

    return 0;
}
