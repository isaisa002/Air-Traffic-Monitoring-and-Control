#include "Logger.h"
#include <iostream>
#include <fstream>
#include <mutex>
#include <sstream>
using namespace std;

static mutex loggerMutex;

// Helper function to log the message to a specific log file based on module
void Logger::logToFile(const string &message, const string &filename) {
    ofstream logFile(filename, ios::app);
    if (logFile.is_open()) {
        logFile << message << endl;
        logFile.close();
    } else {
        cerr << "[Logger] Unable to open log file: " << filename << endl;
    }
}

// Main log function to handle logging based on the module
void Logger::logMessage(const string &message, const string &module) {
    lock_guard<mutex> lock(loggerMutex);

    // Create a log filename based on the module (e.g., "AircraftManager_log.txt")
    stringstream logFilename;
    logFilename << module << "_log.txt";

    // Log to both file and console
    logToFile(message, logFilename.str());
    cout << "[" << module << "] " << message << endl;
}
