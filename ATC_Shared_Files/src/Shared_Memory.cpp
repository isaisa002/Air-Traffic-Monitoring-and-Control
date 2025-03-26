#include "Shared_Memory.h"
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>  // For close()
#include <cstring>
#include "logger.h"

SharedMemory::SharedMemory(bool createFlag) {
    // Open or create the shared memory object
    if (createFlag) {
        fd_ = shm_open("/atc_shared_memory", O_CREAT | O_RDWR, 0666);
        if (fd_ == -1) {
            Logger::logMessage("[SharedMemory] Failed to create shared memory object.");
            exit(1);
        }
        // Resize memory
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

    // Map shared memory into the process's address space
    data_ = (SharedData*) mmap(NULL, sizeof(SharedData), PROT_READ | PROT_WRITE, MAP_SHARED, fd_, 0);
    if (data_ == MAP_FAILED) {
        Logger::logMessage("[SharedMemory] Failed to map shared memory.");
        exit(1);
    }

    // Initialize the mutexes (for thread safety)
    for (int i = 0; i < MAX_AIRCRAFT; i++) {
        pthread_mutex_init(&data_->aircraftLocks[i], NULL);
    }
    pthread_mutex_init(&data_->globalLock, NULL);
}

SharedMemory::~SharedMemory() {
    // Clean up the shared memory
    if (owner_) {
        shm_unlink("/atc_shared_memory");  // Unlink shared memory object
    }
    munmap(data_, sizeof(SharedData));  // Unmap memory
    close(fd_);  // Close file descriptor
}

SharedData* SharedMemory::getData() {
    return data_;
}
