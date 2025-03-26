
#include "Aircraft_Manager.h"

#include "Logger.h"
#include "Timer.h"
#include "Shared_Memory.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include <chrono>
#include <pthread.h>
using namespace std;


// Constructor: initializes the AircraftManager with the given input file path and shared memory pointer.
AircraftManager::AircraftManager(const std::string inputFilePath, SharedMemory* sharedMemory)
    : inputFilePath_(inputFilePath), shm_(sharedMemory)
{
    Logger::logMessage("[AircraftManager] Constructed with input file: " + inputFilePath);
    // Optionally initialize any other members here.
}

// Destructor: Clean up resources if needed.
AircraftManager::~AircraftManager() {
    // Optionally, join threads or free resources.
    Logger::logMessage("[AircraftManager] Destructor called.");
}


void* AircraftManager::aircraftTaskWrapper(void* arg) {
    // Unpack the argument into a suitable object (e.g., an AircraftManager instance)
    auto* data = static_cast<AircraftTaskData*>(arg);
    AircraftManager* manager = data->manager;
    int index = data->index;
    AircraftData initialData = data->aircraft;

    // Call the aircraftTask method on the correct AircraftManager instance
    manager->aircraftTask(index, initialData);

    delete data;  // Clean up memory used for passing arguments
    return nullptr;
}

void AircraftManager::aircraftTask(int index, const AircraftData &initialData) {
    SharedData* data = shm_->getData();

    // Log the start time of the task
    auto taskStart = chrono::steady_clock::now();

    // Lock only the aircraft record for initialization.
    pthread_mutex_lock(&data->aircraftLocks[index]);
    data->aircraftList[index] = initialData;
    data->aircraftList[index].isActive = true;
    pthread_mutex_unlock(&data->aircraftLocks[index]);

    Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) + " started at index " + to_string(index));

    // Periodic update: every second, update position from speed.
    while (true) {
        auto startUpdate = chrono::steady_clock::now();

        pthread_mutex_lock(&data->aircraftLocks[index]);
        data->aircraftList[index].posX += data->aircraftList[index].speedX;
        data->aircraftList[index].posY += data->aircraftList[index].speedY;
        data->aircraftList[index].posZ += data->aircraftList[index].speedZ;
        pthread_mutex_unlock(&data->aircraftLocks[index]);

        auto endUpdate = chrono::steady_clock::now();
        chrono::duration<double> executionTime = endUpdate - startUpdate;
        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) + " updated its position in " + to_string(executionTime.count()) + " seconds.");

        Timer::sleepSeconds(1);  // Update every second

        // Log the execution time after each update
        chrono::duration<double> taskExecutionTime = chrono::steady_clock::now() - taskStart;
        Logger::logMessage("[Aircraft] Aircraft " + to_string(initialData.id) + " task execution time so far: " + to_string(taskExecutionTime.count()) + " seconds.");
    }
}

void AircraftManager::start() {
    ifstream infile(inputFilePath_);
    if (!infile.is_open()) {
        Logger::logMessage("[AircraftManager] Failed to open input file: " + inputFilePath_);
        return;
    }

    // Record simulation start time.
    auto simulationStart = chrono::steady_clock::now();

    string line;
    int index = 0;
    while(getline(infile, line) && index < MAX_AIRCRAFT) {
        istringstream iss(line);
        int releaseTime, id;
        double posX, posY, posZ;
        double speedX, speedY, speedZ;
        double flightLevel;
        if (!(iss >> releaseTime >> id >> posX >> posY >> posZ >> speedX >> speedY >> speedZ >> flightLevel)) {
            Logger::logMessage("[AircraftManager] Skipping invalid line: " + line);
            continue; // Skip invalid lines.
        }

        // Calculate how long to wait until the aircraft's release time.
        auto now = chrono::steady_clock::now();
        int elapsed = chrono::duration_cast<chrono::seconds>(now - simulationStart).count();
        int delay = releaseTime - elapsed;

        // Delay the task execution until the release time.
        if (delay > 0) {
            Logger::logMessage("[AircraftManager] Waiting " + to_string(delay) + " second(s) before starting aircraft task for " + to_string(id));
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
        aircraft.isActive = false;  // Will be set true in the task.
        aircraft.hasResponded = false;  // Initially, the aircraft has not responded to SSR

        // Temporal parameters initialization
        aircraft.releaseTime = chrono::seconds(releaseTime);  // Release time from input
        aircraft.executionTime = chrono::seconds(1);  // Task takes 1 second per update
        aircraft.period = chrono::seconds(1);  // Period of 1 second between tasks (periodic task)
        aircraft.relativeDeadline = aircraft.releaseTime + chrono::seconds(10);  // Relative deadline (10 seconds)
        aircraft.deadline = aircraft.releaseTime + chrono::seconds(15);  // Absolute deadline (15 seconds)

        // Create a new task for the aircraft.
        pthread_t thread;
        auto* data = new AircraftTaskData(this, index, aircraft); // Use the new AircraftTaskData struct
        pthread_create(&thread, nullptr, AircraftManager::aircraftTaskWrapper, data);
        aircraftThreads_.push_back(thread);
        Logger::logMessage("[AircraftManager] Created a new task for aircraft " + to_string(id) +
                           " at index " + to_string(index));
        index++;
    }
}


int main() {
    // Print a hello message to the console.
    cout << "[Aircraft Manager] Hello from Aircraft!" << endl;

    // Set the path to the aircraft input file.
    // Adjust the path as needed—if the file is in the same folder as the executable, this works.
    string inputFilePath = "aircraft_input.txt";

    // Create or initialize the shared memory.
    // Here we assume that the SharedMemory class has a default constructor that
    // handles the initialization (or you could call initSharedMemory(true) if that's your design).
    SharedMemory* sharedMemory = new SharedMemory(true);

    // Create an instance of AircraftManager, passing the input file path and the shared memory pointer.
    AircraftManager manager(inputFilePath, sharedMemory);

    // Start the aircraft manager (this will launch aircraft tasks as per your implementation).
    manager.start();

    // Optionally, you might want to join the threads or wait indefinitely,
    // depending on your design. For now, we'll simply exit.


    return 0;
}
