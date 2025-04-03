#include "Shared_Memory.h"
#include "Logger.h"
#include "Timer.h"
#include <iostream>
#include <string>
#include <cstring>
#include <cstdlib>  // For std::stoi

using namespace std;

int main() {
    Logger::logMessage("[OperatorConsole] Starting operator console...");

    // Open existing shared memory.
    SharedMemory* shm = new SharedMemory(false);

    // Continuously read commands from the controller.
    while (true) {
        cout << "Enter command (e.g., aircraftID:COMMAND parameters, or setn <seconds>): ";
        string command;
        getline(cin, command);

        if (!command.empty()) {
            Logger::logMessage("[OperatorConsole] Received command: " + command, "OperatorConsole");

            // Handle dynamic prediction lookahead command ("setn").
            if (command.rfind("setn ", 0) == 0) {  // command starts with "setn "
                try {
                    int newN = std::stoi(command.substr(5));
                    shm->lockGlobal();
                    shm->getData()->predictionLookahead = newN;
                    shm->unlockGlobal();
                    Logger::logMessage("[OperatorConsole] Updated prediction lookahead to " + to_string(newN));
                } catch (...) {
                    Logger::logMessage("[OperatorConsole] Invalid number for setn command.");
                }
            }
            else {
                // For other commands, log and store in shared memory.
                Logger::logOperatorCommand(command);
                shm->lockGlobal();
                strncpy(shm->getData()->lastOperatorCommand, command.c_str(),
                        sizeof(shm->getData()->lastOperatorCommand) - 1);
                shm->getData()->lastOperatorCommand[sizeof(shm->getData()->lastOperatorCommand) - 1] = '\0'; // Ensure null-termination
                Logger::logMessage("[OperatorConsole] Command written to shared memory.", "OperatorConsole");
                shm->unlockGlobal();
            }
        }
        Timer::sleepSeconds(1);
    }

    delete shm;
    return 0;
}