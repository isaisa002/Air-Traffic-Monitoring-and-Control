#ifndef HISTORYLOGGER_H
#define HISTORYLOGGER_H

#include "Shared_Memory.h"

// HistoryLogger class encapsulates functionality for logging the airspace snapshot.
class HistoryLogger {
public:
    // Logs a snapshot of the current airspace state (all active aircraft data)
    // to a history log file with a timestamp.
    //
    // Parameters:
    // - shm: Pointer to the SharedMemory object containing airspace data.
    //
    // Note: This function should be called periodically (e.g., every 20 seconds)
    //       by a dedicated thread or process.
    static void logAirspaceHistory(SharedMemory* shm);
};

#endif // HISTORYLOGGER_H