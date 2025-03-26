#include "Logger.h"
#include <iostream>
#include <fstream>
#include <mutex>
using namespace std;

static mutex loggerMutex;

void Logger::logMessage(const string &message) {
    lock_guard<mutex> lock(loggerMutex);
    ofstream logFile("atc_log.txt", ios::app);
    if (logFile.is_open()) {
        logFile << message << endl;
        logFile.close();
    } else {
        cerr << "[Logger] Unable to open log file!" << endl;
    }
    cout << "[Logger] " << message << endl;
}
