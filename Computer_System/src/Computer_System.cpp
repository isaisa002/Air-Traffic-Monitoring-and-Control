#include <iostream>
#include <chrono>
#include <cmath>
#include <limits>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"


using namespace std;
using namespace std::chrono;

// Separation thresholds
const double MIN_HORIZONTAL_SEPARATION = 3000.0;
const double MIN_VERTICAL_SEPARATION = 1000.0;

// Function to perform a safety check using predicted positions.
// predictionTime: seconds ahead to predict each aircraft's position.
bool checkSafety(SharedMemory* shm, int predictionTime) {
    SharedData* data = shm->getData();
    bool safetyOk = true;

    // Lock the global shared memory data while scanning
    shm->lockGlobal();

    // Loop over each active aircraft to calculate predicted positions.
    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        if (!data->aircraftList[i].isActive)
            continue;

        // Predicted position for aircraft i
        double predX_i = data->aircraftList[i].posX + data->aircraftList[i].speedX * predictionTime;
        double predY_i = data->aircraftList[i].posY + data->aircraftList[i].speedY * predictionTime;
        double predZ_i = data->aircraftList[i].posZ + data->aircraftList[i].speedZ * predictionTime;

        // Compare with every other active aircraft
        for (int j = i + 1; j < MAX_AIRCRAFT; j++) {
            if (!data->aircraftList[j].isActive)
                continue;

            double predX_j = data->aircraftList[j].posX + data->aircraftList[j].speedX * predictionTime;
            double predY_j = data->aircraftList[j].posY + data->aircraftList[j].speedY * predictionTime;
            double predZ_j = data->aircraftList[j].posZ + data->aircraftList[j].speedZ * predictionTime;

            // Compute horizontal distance using Euclidean distance in the X-Y plane
            double horizontalDistance = sqrt(pow(predX_i - predX_j, 2) + pow(predY_i - predY_j, 2));
            // Compute vertical separation
            double verticalDistance = fabs(predZ_i - predZ_j);

            // Check if either separation requirement is violated
            if (horizontalDistance < MIN_HORIZONTAL_SEPARATION || verticalDistance < MIN_VERTICAL_SEPARATION) {
                Logger::logMessage("[ComputerSystem] Safety violation detected between aircraft " +
                                     to_string(data->aircraftList[i].id) + " and " +
                                     to_string(data->aircraftList[j].id));
                safetyOk = false;
                // Optionally, you can continue to log additional violations here.
            }
        }
    }
    shm->unlockGlobal();
    return safetyOk;
}

// Function to generate an alert when a safety violation is detected.
void generateAlert() {
    // Here, you might trigger a visual, auditory, or other notification.
    Logger::logMessage("[ComputerSystem] ALERT: Separation violation detected!");
    // Additional alert logic (e.g., sending notifications to the Operator Console) can be added here.
}

// Function to perform scheduling feasibility tests.
// It repeatedly runs the safety check and measures execution times.
void performSchedulingFeasibilityTest(SharedMemory* shm, int predictionTime, int numIterations) {
    double bestTime = numeric_limits<double>::max();
    double worstTime = 0.0;
    double totalTime = 0.0;

    for (int i = 0; i < numIterations; i++) {
        auto start = high_resolution_clock::now();
        // Execute safety check
        checkSafety(shm, predictionTime);
        auto end = high_resolution_clock::now();

        double duration = duration_cast<microseconds>(end - start).count() / 1000.0;  // in milliseconds
        bestTime = min(bestTime, duration);
        worstTime = max(worstTime, duration);
        totalTime += duration;

        Logger::logMessage("[ComputerSystem] Scheduling test iteration " + to_string(i) +
                           ": " + to_string(duration) + " ms");
        Timer::sleepMilliseconds(100);  // Short delay between iterations.
    }

    double avgTime = totalTime / numIterations;
    Logger::logMessage("[ComputerSystem] Scheduling Feasibility Test Results:"
                       " Best: " + to_string(bestTime) + " ms, "
                       "Worst: " + to_string(worstTime) + " ms, "
                       "Avg: " + to_string(avgTime) + " ms");
}

int main() {
    cout << "[Computer_System] Starting Computer System..." << endl;

    // Create shared memory (true: create and initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Computer_System] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Computer_System] Shared memory opened successfully." << endl;

    // You can optionally run the scheduling feasibility test once at startup:
    int predictionTime = 60;  // e.g., predict 60 seconds ahead
    performSchedulingFeasibilityTest(shm, predictionTime, 10);

    // Log a test message using Logger's static method
    Logger::logMessage("[Computer_System] Test log entry from Computer_System.");
    cout << "[Computer_System] Logger wrote to file successfully." << endl;

    // Clean up shared memory when done
    delete shm;
    return 0;
}
