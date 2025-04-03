#include "Shared_Memory.h"
#include "Logger.h"
#include "Timer.h"
#include <fstream>
#include <ctime>
#include <sstream>
#include <iostream>

void logHistory(SharedMemory* shm) {
    SharedData* data = shm->getData();
    std::ofstream historyFile("airspace_history.log", std::ios::app);
    if (historyFile.is_open()) {
        std::time_t now = std::time(nullptr);
        std::stringstream timeStream;
        timeStream << std::ctime(&now);  // ctime() includes a newline
        historyFile << "Timestamp: " << timeStream.str();
        for (int i = 0; i < MAX_AIRCRAFT; ++i) {
            shm->lockAircraft(i);
            historyFile << "Aircraft " << i << " (ID " << data->aircraftList[i].id << "): ";
            historyFile << "Pos(" << data->aircraftList[i].posX << ", "
                        << data->aircraftList[i].posY << ", "
                        << data->aircraftList[i].posZ << ") ";
            historyFile << "Speed(" << data->aircraftList[i].speedX << ", "
                        << data->aircraftList[i].speedY << ", "
                        << data->aircraftList[i].speedZ << ")\n";
            shm->unlockAircraft(i);
        }
        historyFile << "---------------------\n";
        historyFile.close();
    } else {
        Logger::logMessage("[HistoryLogger] ERROR: Unable to open history log file.");
    }
}

int main() {
    Logger::logMessage("[HistoryLogger] Starting History Logger...");
    SharedMemory* shm = new SharedMemory(false);

    while (true) {
        logHistory(shm);
        Timer::sleepSeconds(20); // Log history every 20 seconds.
    }

    delete shm;
    return 0;
}