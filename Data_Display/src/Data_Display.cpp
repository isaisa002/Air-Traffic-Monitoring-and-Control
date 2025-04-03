#include <iostream>
#include <vector>
#include <iomanip>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"

using namespace std;

const int GRID_WIDTH = 80;   // Number of columns
const int GRID_HEIGHT = 20;  // Number of rows
const double AIRSPACE_SIZE = 100000.0; // Assumed square airspace

void clearScreen() {
    cout << "\033[2J\033[1;1H";
}

void updateDisplay(SharedMemory* shm) {
    clearScreen();
    vector<vector<char>> grid(GRID_HEIGHT, vector<char>(GRID_WIDTH, '.'));
    double xScale = AIRSPACE_SIZE / GRID_WIDTH;
    double yScale = AIRSPACE_SIZE / GRID_HEIGHT;
    SharedData* data = shm->getData();

    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        shm->lockAircraft(i);
        if (data->aircraftList[i].isActive) {
            int col = static_cast<int>(data->aircraftList[i].posX / xScale);
            int row = static_cast<int>(data->aircraftList[i].posY / yScale);
            if (col >= 0 && col < GRID_WIDTH && row >= 0 && row < GRID_HEIGHT) {
                // Optionally, display aircraft ID instead of a generic marker.
                grid[row][col] = 'A';
            }
        }
        shm->unlockAircraft(i);
    }

    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            cout << grid[row][col];
        }
        cout << endl;
    }
}

int main() {
    Logger::logMessage("[DataDisplay] Starting Data Display subsystem...");
    SharedMemory* shm = new SharedMemory(false);

    while (true) {
        updateDisplay(shm);
        Timer::sleepSeconds(5); // Refresh every 5 seconds.
    }

    delete shm;
    return 0;
}