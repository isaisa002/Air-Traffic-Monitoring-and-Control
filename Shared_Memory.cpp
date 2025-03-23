#include "Shared_Memory.h"
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <iostream>
using namespace std;

// Function to initialize shared memory
SharedMemoryStruct* initSharedMemory(bool create) {
	cout << "[Shared Memory] Entered shared memory\n";

    int shm_fd = shm_open(SHM_NAME, create ? (O_CREAT | O_RDWR) : O_RDWR, 0666);
    if (shm_fd == -1) {
        perror("shm_open");
        return nullptr;
    }

    // Set the size of shared memory
    ftruncate(shm_fd, sizeof(SharedMemoryStruct));

    // Map shared memory to address space
    void* ptr = mmap(0, sizeof(SharedMemoryStruct), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (ptr == MAP_FAILED) {
        perror("mmap");
        return nullptr;
    }

    // Cast the mapped memory to SharedMemoryStruct pointer
    SharedMemoryStruct* shm = static_cast<SharedMemoryStruct*>(ptr);

    if (create) {
        // Initialize shared memory if creating new
        pthread_mutexattr_t attr;
        pthread_mutexattr_init(&attr);
        pthread_mutexattr_setpshared(&attr, PTHREAD_PROCESS_SHARED);
        pthread_mutex_init(&shm->mutex, &attr);
        shm->aircraft_count = 0;
        shm->lookahead_seconds = 60;
        shm->alert_triggered = false;
    }

    return shm;
}
