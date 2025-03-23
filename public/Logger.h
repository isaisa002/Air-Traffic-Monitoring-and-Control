#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>

// Logger class for logging messages to a file
class Logger {
public:
    // Constructor: opens the log file for appending
    Logger(const std::string& filename);

    // Destructor: closes the log file
    ~Logger();

    // Log a message to the file
    void log(const std::string& entry);

private:
    std::ofstream log_file;  // File stream for logging
};

#endif
