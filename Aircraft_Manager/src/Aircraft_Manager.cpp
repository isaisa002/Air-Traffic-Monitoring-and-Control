#include "Aircraft_Manager.h"
#include "Logger.h"
#include "Timer.h"
#include "Shared_Memory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <pthread.h>
#include <cstring>    // For strcmp
#include <vector>
using namespace std;

// Constructor: initializes AircraftManager with the given input file path and shared memory pointer.
AircraftManager::AircraftManager(const std::string inputFilePath, SharedMemory* sharedMemory)
    : inputFilePath_(inputFilePath), shm_(sharedMemory)
{
    // Log the construction of the AircraftManager
    Logger::logMessage("[AircraftManager] Constructed with input file: " + inputFilePath, "AircraftManager");
}

AircraftManager::~AircraftManager() {
    // Log the destruction of the AircraftManager
    Logger::logMessage("[AircraftManager] Destructor called.", "AircraftManager");
}

// Thread wrapper that unpacks the argument and calls aircraftTask.
void* AircraftManager::aircraftTaskWrapper(void* arg) {
    auto* data = static_cast<AircraftTaskData*>(arg);
    AircraftManager* manager = data->manager;
    int index = data->index;
    AircraftData initialData = data->aircraft;

    manager->aircraftTask(index, initialData);
    delete data;
    return nullptr;
}

// The aircraft task: updates the aircraft's position every second.
void AircraftManager::aircraftTask(int index, const AircraftData &initialData) {
    SharedData* sharedData = shm_->getData();
    auto taskStart = chrono::steady_clock::now();

    // Acquire lock for this aircraft and initialize its data.
    shm_->lockAircraft(index);
    sharedData->aircraftList[index] = initialData;
    sharedData->aircraftList[index].isActive = true;
    shm_->unlockAircraft(index);

    Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) +
                       " started at index " + to_string(index), "AircraftManager");

    while (true) {
        auto startUpdate = chrono::steady_clock::now();

        // Calculate new position based on speed and previous position
        double newPosX = sharedData->aircraftList[index].posX + sharedData->aircraftList[index].speedX;
        double newPosY = sharedData->aircraftList[index].posY + sharedData->aircraftList[index].speedY;
        double newPosZ = sharedData->aircraftList[index].posZ + sharedData->aircraftList[index].speedZ;

        // Call the updatePosition function to update the aircraft's position
        updatePosition(sharedData->aircraftList[index].id, newPosX, newPosY, newPosZ);

        auto endUpdate = chrono::steady_clock::now();
        chrono::duration<double> executionTime = endUpdate - startUpdate;
        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) +
                           " updated its position in " + to_string(executionTime.count()) + " seconds.", "AircraftManager");

        Timer::sleepSeconds(10);  // Wait 1 second

        chrono::duration<double> taskExecutionTime = chrono::steady_clock::now() - taskStart;
        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) +
                           " task execution time so far: " + to_string(taskExecutionTime.count()) + " seconds.", "AircraftManager");
    }
}


// Updates the speed of an aircraft with the given id.
void AircraftManager::updateSpeed(int aircraftId, double newSpeedX, double newSpeedY, double newSpeedZ) {
    SharedData* sharedData = shm_->getData();
    // Iterate over aircraftList to find the matching aircraft.
    for (int i = 0; i < MAX_AIRCRAFT; ++i) {
        shm_->lockAircraft(i);
        if (sharedData->aircraftList[i].isActive && sharedData->aircraftList[i].id == aircraftId) {
            // Log the speed update process
            Logger::logMessage("[AircraftManager] Updating speed for aircraft " + to_string(aircraftId), "AircraftManager");

            // Update the aircraft's speed
            sharedData->aircraftList[i].speedX = newSpeedX;
            sharedData->aircraftList[i].speedY = newSpeedY;
            sharedData->aircraftList[i].speedZ = newSpeedZ;

            // Log the new speed after the update
            Logger::logMessage("[AircraftManager] New speed for aircraft " + to_string(aircraftId) + " is: " +
                               "SpeedX = " + to_string(newSpeedX) + ", SpeedY = " + to_string(newSpeedY) + ", SpeedZ = " + to_string(newSpeedZ), "AircraftManager");

            shm_->unlockAircraft(i);
            return;
        }
        shm_->unlockAircraft(i);
    }

    // Log if the aircraft was not found for speed update
    Logger::logMessage("[AircraftManager] Aircraft " + to_string(aircraftId) + " not found for speed update.", "AircraftManager");
}


// Updates the position of an aircraft with the given id.
void AircraftManager::updatePosition(int aircraftId, double newPosX, double newPosY, double newPosZ) {
    SharedData* sharedData = shm_->getData();
    for (int i = 0; i < MAX_AIRCRAFT; ++i) {
        shm_->lockAircraft(i);
        if (sharedData->aircraftList[i].isActive && sharedData->aircraftList[i].id == aircraftId) {
            // Log the position update process
            Logger::logMessage("[AircraftManager] Updating position for aircraft " + to_string(aircraftId), "AircraftManager");

            // Update the aircraft's position
            sharedData->aircraftList[i].posX = newPosX;
            sharedData->aircraftList[i].posY = newPosY;
            sharedData->aircraftList[i].posZ = newPosZ;

            // Log the new position after the update
            Logger::logMessage("[AircraftManager] New position for aircraft " + to_string(aircraftId) + " is: " +
                               "X = " + to_string(newPosX) + ", Y = " + to_string(newPosY) + ", Z = " + to_string(newPosZ), "AircraftManager");

            shm_->unlockAircraft(i);
            return;
        }
        shm_->unlockAircraft(i);
    }

    // Log if the aircraft was not found for position update
    Logger::logMessage("[AircraftManager] Aircraft " + to_string(aircraftId) + " not found for position update.", "AircraftManager");
}

// Process commands that are sent from shared memory
void AircraftManager::processCommand() {
    SharedData* sharedData = shm_->getData();
    shm_->lockGlobal();

    string command(sharedData->lastOperatorCommand);
    if (command.empty()) {
        shm_->unlockGlobal();
        return;
    }

    // Process command based on format: "SET_SPEED 1001 300 0 0"
    istringstream iss(command);
    string cmd;
    int aircraftId;
    iss >> cmd >> aircraftId;

    if (cmd == "SET_SPEED") {
        double newSpeedX, newSpeedY, newSpeedZ;
        iss >> newSpeedX >> newSpeedY >> newSpeedZ;
        updateSpeed(aircraftId, newSpeedX, newSpeedY, newSpeedZ);
    } else if (cmd == "SET_POSITION") {
        double newPosX, newPosY, newPosZ;
        iss >> newPosX >> newPosY >> newPosZ;
        updatePosition(aircraftId, newPosX, newPosY, newPosZ);
    }

    // Clear the command after processing
    memset(sharedData->lastOperatorCommand, 0, sizeof(sharedData->lastOperatorCommand));
    shm_->unlockGlobal();
}

// Start method: reads the input file and creates tasks for each aircraft.
void AircraftManager::start() {
    ifstream infile(inputFilePath_);
    if (!infile.is_open()) {
        Logger::logMessage("[AircraftManager] Failed to open input file: " + inputFilePath_, "AircraftManager");
        return;
    }

    auto simulationStart = chrono::steady_clock::now();
    string line;
    int index = 0;

    while (getline(infile, line) && index < MAX_AIRCRAFT) {
        istringstream iss(line);
        int releaseTime, id;
        double posX, posY, posZ;
        double speedX, speedY, speedZ;
        double flightLevel;
        if (!(iss >> releaseTime >> id >> posX >> posY >> posZ >> speedX >> speedY >> speedZ >> flightLevel)) {
            Logger::logMessage("[AircraftManager] Skipping invalid line: " + line, "AircraftManager");
            continue;
        }

        // Calculate delay until release time.
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - simulationStart).count();
        int delay = releaseTime - elapsed;
        if (delay > 0) {
            Logger::logMessage("[AircraftManager] Waiting " + to_string(delay) +
                               " second(s) before starting aircraft task for " + to_string(id), "AircraftManager");
            Timer::sleepSeconds(delay);
        }

        AircraftData aircraft;
        aircraft.id = id;
        aircraft.posX = posX;
        aircraft.posY = posY;
        aircraft.posZ = posZ;
        aircraft.speedX = speedX;
        aircraft.speedY = speedY;
        aircraft.speedZ = speedZ;
        aircraft.flightLevel = flightLevel;
        aircraft.isActive = false;
        aircraft.hasResponded = false;

        // Temporal parameters.
        aircraft.releaseTime = chrono::seconds(releaseTime);
        aircraft.executionTime = chrono::seconds(1);
        aircraft.period = chrono::seconds(1);

        pthread_t thread;
        auto* data = new AircraftTaskData(this, index, aircraft);
        pthread_create(&thread, nullptr, AircraftManager::aircraftTaskWrapper, data);
        pthread_detach(thread);  // Detach thread to free resources.
        aircraftThreads_.push_back(thread);

        Logger::logMessage("[AircraftManager] Created a new task for aircraft " + to_string(id) +
                           " at index " + to_string(index), "AircraftManager");
        index++;
    }
}

int main() {
    cout << "[Aircraft Manager] Hello from Aircraft!" << endl;

    // Initialize the file path and shared memory object
    string inputFilePath = "aircraft_input.txt";
    SharedMemory* sharedMemory = new SharedMemory(true);  // Create shared memory

    // Instantiate the AircraftManager with the shared memory and input file path
    AircraftManager manager(inputFilePath, sharedMemory);

    // Start the aircraft manager tasks. This will launch aircraft tasks in separate threads.
    manager.start();

    // Process commands continuously from shared memory
    // The command loop will interact with the operator console
    while (true) {
        // Process any commands that were written to shared memory by the operator console or communication system
        manager.processCommand();

        // Sleep for a short period to prevent high CPU usage
        Timer::sleepSeconds(1);
    }

    // Cleanup (though this part is unlikely to be reached due to the infinite loop)
    delete sharedMemory;
    return 0;
}
