#include "Shared_Memory.h"
#include "Logger.h"
#include "Timer.h"
#include <iostream>
#include <string>
#include <cstring>
using namespace std;

int main() {
    Logger::logMessage("[OperatorConsole] Starting operator console...");

    // Open existing shared memory (do not create a new one)
    SharedMemory* shm = new SharedMemory(false);

    // Continuously read commands from the controller (user input)
    while (true) {
        cout << "Enter command (format: aircraftID:COMMAND parameters): ";
        string command;
        getline(cin, command);

        if (!command.empty()) {
            Logger::logMessage("[OperatorConsole] Received command: " + command, "OperatorConsole");
        
            // Log to dedicated operator commands log file.
            Logger::logOperatorCommand(command);
        
            // Write the command to the shared memory's lastOperatorCommand field.
            // We use the global lock since lastOperatorCommand is shared data.
            shm->lockGlobal();

            // Clear any previous command and write the new command.
            // Ensure we don't overflow the 256-char buffer.
            strncpy(shm->getData()->lastOperatorCommand, command.c_str(), sizeof(shm->getData()->lastOperatorCommand) - 1);
            shm->getData()->lastOperatorCommand[sizeof(shm->getData()->lastOperatorCommand) - 1] = '\0';  // Ensure null-termination

            Logger::logMessage("[OperatorConsole] Command written to shared memory.", "OperatorConsole");
            shm->unlockGlobal();
        
        // Optionally, add a short delay before allowing the next command.
            Timer::sleepSeconds(1);
        }
    }

    return 0;
}
