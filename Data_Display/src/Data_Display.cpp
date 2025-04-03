#include <iostream>
#include <vector>
#include <iomanip>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"


using namespace std;

const int GRID_WIDTH = 80;   // number of columns in the grid
const int GRID_HEIGHT = 20;  // number of rows in the grid
const double AIRSPACE_SIZE = 100000.0; // assuming square airspace for X and Y

// Function to clear the terminal screen using ANSI escape codes.
void clearScreen() {
    // ANSI escape sequence to clear screen and reposition cursor to top-left.
    cout << "\033[2J\033[1;1H";
}

// Function to update and render the airspace plan view.
void updateDisplay(SharedMemory* shm) {
    clearScreen();
    // Create a grid filled with dots to represent empty space.
    vector<vector<char>> grid(GRID_HEIGHT, vector<char>(GRID_WIDTH, '.'));

    // Calculate scaling factors: each grid cell represents a block in the airspace.
    double xScale = AIRSPACE_SIZE / GRID_WIDTH;
    double yScale = AIRSPACE_SIZE / GRID_HEIGHT;

    SharedData* data = shm->getData();

    // Iterate through each aircraft slot.
    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        // Lock aircraft data before accessing.
        shm->lockAircraft(i);
        if (data->aircraftList[i].isActive) {
            // Compute the grid cell indices.
            int col = static_cast<int>(data->aircraftList[i].posX / xScale);
            int row = static_cast<int>(data->aircraftList[i].posY / yScale);

            // Ensure indices are within grid bounds.
            if (col >= 0 && col < GRID_WIDTH && row >= 0 && row < GRID_HEIGHT) {
                // Mark the aircraft position with an 'A'. (You could also include aircraft ID.)
                grid[row][col] = 'A';
            }
        }
        shm->unlockAircraft(i);
    }

    // Print the grid to the terminal.
    for (int row = 0; row < GRID_HEIGHT; ++row) {
        for (int col = 0; col < GRID_WIDTH; ++col) {
            cout << grid[row][col];
        }
        cout << endl;
    }
    cout.flush();
}

int main() {
    cout << "[Data_Display] Starting enhanced Data Display..." << endl;

    // Create or open shared memory (true here means create & initialize)
    SharedMemory* shm = new SharedMemory(true);
    if (!shm) {
        cerr << "[Data_Display] Failed to open shared memory." << endl;
        return 1;
    }
    cout << "[Data_Display] Shared memory opened successfully." << endl;

    // Continuously update the display every 5 seconds.
    while (true) {
        updateDisplay(shm);
        Timer::sleepSeconds(5);
    }

    // Cleanup (this point is never reached due to the infinite loop).
    delete shm;
    return 0;
}
