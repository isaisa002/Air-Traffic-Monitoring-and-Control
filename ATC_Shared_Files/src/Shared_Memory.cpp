#include "Shared_Memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>  // For close()
#include <cstring>
#include "Logger.h"

SharedMemory::SharedMemory(bool createFlag) {
    if (createFlag) {
        fd_ = shm_open("/atc_shared_memory", O_CREAT | O_RDWR, 0666);
        if (fd_ == -1) {
            Logger::logMessage("[SharedMemory] Failed to create shared memory object.");
            exit(1);
        }
        if (ftruncate(fd_, sizeof(SharedData)) == -1) {
            Logger::logMessage("[SharedMemory] Failed to set the size of shared memory.");
            exit(1);
        }
        owner_ = true;
    } else {
        fd_ = shm_open("/atc_shared_memory", O_RDWR, 0666);
        if (fd_ == -1) {
            Logger::logMessage("[SharedMemory] Failed to open shared memory object.");
            exit(1);
        }
        owner_ = false;
    }

    data_ = static_cast<SharedData*>(mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0));
    if (data_ == MAP_FAILED) {
        Logger::logMessage("[SharedMemory] Failed to map shared memory.");
        exit(1);
    }
    Logger::logMessage("[SharedMemory] Shared memory mapped at address: " +
                       std::to_string(reinterpret_cast<uintptr_t>(data_)));

    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        Logger::logMessage("[SharedMemory] Initializing lock for aircraft index " + std::to_string(i) +
                           " at address " + std::to_string(reinterpret_cast<uintptr_t>(&data_->aircraftLocks[i])));
        pthread_mutex_init(&data_->aircraftLocks[i], nullptr);
    }
    Logger::logMessage("[SharedMemory] Initializing global lock at address " +
                       std::to_string(reinterpret_cast<uintptr_t>(&data_->globalLock)));
    pthread_mutex_init(&data_->globalLock, nullptr);
}

SharedMemory::~SharedMemory() {
    if (owner_) {
        Logger::logMessage("[SharedMemory] Unlinking shared memory object.");
        shm_unlink("/atc_shared_memory");
    }
    munmap(data_, sizeof(SharedData));
    close(fd_);
}

SharedData* SharedMemory::getData() {
    Logger::logMessage("[SharedMemory] Accessing shared memory at address: " +
                       std::to_string(reinterpret_cast<uintptr_t>(data_)));
    return data_;
}

void SharedMemory::lockAircraft(int index) {
    Logger::logMessage("[SharedMemory] Attempting to acquire lock for aircraft index " + std::to_string(index) +
                       " at address " + std::to_string(reinterpret_cast<uintptr_t>(&data_->aircraftLocks[index])));
    pthread_mutex_lock(&data_->aircraftLocks[index]);
    Logger::logMessage("[SharedMemory] Acquired lock for aircraft index " + std::to_string(index));
}

void SharedMemory::unlockAircraft(int index) {
    pthread_mutex_unlock(&data_->aircraftLocks[index]);
    Logger::logMessage("[SharedMemory] Released lock for aircraft index " + std::to_string(index));
}

void SharedMemory::lockGlobal() {
   // Logger::logMessage("[SharedMemory] Attempting to acquire global lock at address " +
                     //  std::to_string(reinterpret_cast<uintptr_t>(&data_->globalLock)));
    pthread_mutex_lock(&data_->globalLock);
   // Logger::logMessage("[SharedMemory] Acquired global lock.");
}

void SharedMemory::unlockGlobal() {
    pthread_mutex_unlock(&data_->globalLock);
  //  Logger::logMessage("[SharedMemory] Released global lock.");
}
