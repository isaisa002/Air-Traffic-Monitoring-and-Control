#include "Logger.h"
#include <fstream>
#include <iostream>

// Constructor: opens the log file for appending
Logger::Logger(const std::string& filename) : log_file(filename, std::ios::app) {
    if (!log_file.is_open()) {
        std::cerr << "[Logger] Failed to open log file." << std::endl;
    }
}

// Destructor: closes the log file
Logger::~Logger() {
    if (log_file.is_open()) {
        log_file.close();
    }
}

// Log a message to the file
void Logger::log(const std::string& entry) {
    if (log_file.is_open()) {
        log_file << entry << std::endl;
    } else {
        std::cerr << "[Logger] Log file not open." << std::endl;
    }
}
