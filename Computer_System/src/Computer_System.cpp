#include <iostream>
#include <chrono>
#include <cmath>
#include <limits>
#include "Shared_Memory.h"
#include "Timer.h"
#include "Logger.h"
#include <sstream>

using namespace std;
using namespace std::chrono;

const double MIN_HORIZONTAL_SEPARATION = 3000.0;
const double MIN_VERTICAL_SEPARATION = 1000.0;

bool checkSafety(SharedMemory* shm, int predictionTime) {
    SharedData* data = shm->getData();
    bool safetyOk = true;

    shm->lockGlobal();
    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        if (!data->aircraftList[i].isActive)
            continue;

        double predX_i = data->aircraftList[i].posX + data->aircraftList[i].speedX * predictionTime;
        double predY_i = data->aircraftList[i].posY + data->aircraftList[i].speedY * predictionTime;
        double predZ_i = data->aircraftList[i].posZ + data->aircraftList[i].speedZ * predictionTime;

        for (int j = i + 1; j < MAX_AIRCRAFT; j++) {
            if (!data->aircraftList[j].isActive)
                continue;

            double predX_j = data->aircraftList[j].posX + data->aircraftList[j].speedX * predictionTime;
            double predY_j = data->aircraftList[j].posY + data->aircraftList[j].speedY * predictionTime;
            double predZ_j = data->aircraftList[j].posZ + data->aircraftList[j].speedZ * predictionTime;

            double horizontalDistance = sqrt(pow(predX_i - predX_j, 2) + pow(predY_i - predY_j, 2));
            double verticalDistance = fabs(predZ_i - predZ_j);

            if (horizontalDistance < MIN_HORIZONTAL_SEPARATION || verticalDistance < MIN_VERTICAL_SEPARATION) {
                safetyOk = false;
                Logger::logMessage("[ComputerSystem] WARNING: Predicted safety violation between Aircraft " +
                                   to_string(data->aircraftList[i].id) + " and Aircraft " +
                                   to_string(data->aircraftList[j].id) + " in " + to_string(predictionTime) + " seconds.");
            }
        }
    }
    shm->unlockGlobal();
    return safetyOk;
}

int main() {
    Logger::logMessage("[ComputerSystem] Starting Computer System...");
    SharedMemory* shm = new SharedMemory(false);

    while (true) {
        // Retrieve the dynamic prediction lookahead.
        shm->lockGlobal();
        int n = shm->getData()->predictionLookahead;
        shm->unlockGlobal();

        if (!checkSafety(shm, n)) {
            // Trigger an alarm if violation is predicted within 2 minutes.
            if (n <= 120) {
                Logger::logMessage("[ComputerSystem] ALARM: Safety violation predicted within 2 minutes!");
                // Optionally: add sound or visual alert mechanism.
            }
        }
        Timer::sleepSeconds(1);
    }

    delete shm;
    return 0;
}
