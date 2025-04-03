#include "Shared_Memory.h"
#include <cstring>
#include <cstdlib>
#include <iostream>

SharedMemory::SharedMemory(bool createNew) {
    // For simplicity, allocate memory on the heap.
    data_ = new SharedData;
    std::memset(data_, 0, sizeof(SharedData));
    
    // Initialize global lock.
    pthread_mutex_init(&data_->globalLock, NULL);
    // Initialize each aircraft lock.
    for (int i = 0; i < MAX_AIRCRAFT; ++i) {
        pthread_mutex_init(&data_->aircraftLocks[i], NULL);
    }
    // Set default prediction lookahead (e.g., 60 seconds)
    data_->predictionLookahead = 60;
}

SharedMemory::~SharedMemory() {
    pthread_mutex_destroy(&data_->globalLock);
    for (int i = 0; i < MAX_AIRCRAFT; ++i) {
        pthread_mutex_destroy(&data_->aircraftLocks[i]);
    }
    delete data_;
}

SharedData* SharedMemory::getData() {
    return data_;
}

void SharedMemory::lockGlobal() {
    pthread_mutex_lock(&data_->globalLock);
}

void SharedMemory::unlockGlobal() {
    pthread_mutex_unlock(&data_->globalLock);
}

void SharedMemory::lockAircraft(int index) {
    pthread_mutex_lock(&data_->aircraftLocks[index]);
}

void SharedMemory::unlockAircraft(int index) {
    pthread_mutex_unlock(&data_->aircraftLocks[index]);
}
