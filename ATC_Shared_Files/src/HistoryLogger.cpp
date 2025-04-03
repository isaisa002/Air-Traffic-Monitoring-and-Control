#include "Shared_Memory.h"
#include "Logger.h"
#include "Timer.h"
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>

void logAirspaceHistory(SharedMemory* shm) {
    // Open the history log file in append mode.
    std::ofstream historyFile("AirspaceHistory_log.txt", std::ios::app);
    if (!historyFile.is_open()) {
        Logger::logMessage("[HistoryLogger] Unable to open history log file.");
        return;
    }

    // Get current time as a formatted string.
    auto now = std::chrono::system_clock::now();
    std::time_t now_time = std::chrono::system_clock::to_time_t(now);
    std::stringstream timeStream;
    timeStream << std::put_time(std::localtime(&now_time), "%Y-%m-%d %H:%M:%S");

    // Write header with timestamp.
    historyFile << "----- Airspace Snapshot at " << timeStream.str() << " -----\n";

    SharedData* data = shm->getData();
    shm->lockGlobal();  // Ensure safe reading of all aircraft data.

    // Loop through each aircraft slot.
    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        if (data->aircraftList[i].isActive) {
            historyFile << "Aircraft " << data->aircraftList[i].id
                        << " | Pos(" << data->aircraftList[i].posX << ", "
                        << data->aircraftList[i].posY << ", "
                        << data->aircraftList[i].posZ << ")"
                        << " | Speed(" << data->aircraftList[i].speedX << ", "
                        << data->aircraftList[i].speedY << ", "
                        << data->aircraftList[i].speedZ << ")"
                        << " | Flight Level: " << data->aircraftList[i].flightLevel << "\n";
        }
    }
    shm->unlockGlobal();

    historyFile << "-----------------------------------------------\n\n";
    historyFile.close();
}

int main() {
    // Open shared memory (assume this process is responsible solely for history logging)
    SharedMemory* shm = new SharedMemory(false);
    if (!shm) {
        Logger::logMessage("[HistoryLogger] Failed to open shared memory.");
        return 1;
    }

    Logger::logMessage("[HistoryLogger] History Logger started.");

    // Continuously log airspace history every 20 seconds.
    while (true) {
        logAirspaceHistory(shm);
        Timer::sleepSeconds(20);
    }

    // Cleanup (this will likely never be reached due to the infinite loop)
    delete shm;
    return 0;
}