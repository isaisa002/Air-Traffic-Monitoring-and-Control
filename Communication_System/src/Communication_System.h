#ifndef COMMUNICATION_SYSTEM_H
#define COMMUNICATION_SYSTEM_H

#include <string>
#include "Shared_Memory.h"
using namespace std;

class CommunicationSystem {
public:
    CommunicationSystem(SharedMemory* sharedMemory);
    void sendCommand(int aircraftID, const string& command);
    void receiveData(int aircraftID);

private:
    SharedMemory* shm_;
};

#endif // COMMUNICATION_SYSTEM_H
