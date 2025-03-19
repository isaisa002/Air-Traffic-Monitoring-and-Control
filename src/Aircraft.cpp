#include "Aircraft.h"

// Constructor
Aircraft::Aircraft(int id, double x, double y, double z, double speedX, double speedY, double speedZ)
    : id(id), x(x), y(y), z(z), speedX(speedX), speedY(speedY), speedZ(speedZ) {
    pthread_mutex_init(&mtx, NULL);
}

// Get Aircraft ID
int Aircraft::getID() const {
    return id;
}

// Update location based on speed
void Aircraft::updateLocation() {
    pthread_mutex_lock(&mtx);
    x += speedX;
    y += speedY;
    z += speedZ;
    pthread_mutex_unlock(&mtx);
}

// Respond to radar request
string Aircraft::getRadarInfo() {
    pthread_mutex_lock(&mtx);
    string info = "Aircraft ID: " + to_string(id) +
                  " Position: (" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ") " +
                  " Speed: (" + to_string(speedX) + ", " + to_string(speedY) + ", " + to_string(speedZ) + ")";
    pthread_mutex_unlock(&mtx);
    return info;
}

// Process ATC command to modify speed or altitude
void Aircraft::processCommand(string command, double value) {
    pthread_mutex_lock(&mtx);
    if (command == "change_speedX") speedX = value;
    else if (command == "change_speedY") speedY = value;
    else if (command == "change_speedZ") speedZ = value;
    else if (command == "change_altitude") z = value;
    pthread_mutex_unlock(&mtx);
}

// Start real-time aircraft simulation using POSIX thread
void* Aircraft::startSimulation(void* arg) {
    Aircraft* aircraft = (Aircraft*)arg;
    while (true) {
        aircraft->updateLocation();
        usleep(1000000); // Sleep for 1 second
    }
    return NULL;
}

// Log aircraft status using shared memory
void Aircraft::logStatus() {
    int shm_fd = shm_open(SHARED_MEMORY_NAME, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, 1024);
    void* ptr = mmap(0, 1024, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    while (true) {
        pthread_mutex_lock(&mtx);
        string logData = "Aircraft " + to_string(id) +
                         " Position: (" + to_string(x) + ", " + to_string(y) + ", " + to_string(z) + ")";
        memcpy(ptr, logData.c_str(), logData.length());
        pthread_mutex_unlock(&mtx);
        usleep(20000000); // Log every 20 seconds
    }
}
