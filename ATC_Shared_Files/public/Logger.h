#ifndef LOGGER_H
#define LOGGER_H

#include <string>
using namespace std;

class Logger {
public:
    // Log message to different destinations (e.g., file or console) based on the module
    static void logMessage(const string &message, const string &module = "General");

    // Log operator command
    static void logOperatorCommand(const std::string &command);

private:
    // Private helper method to log to a specific file
    static void logToFile(const string &message, const string &filename);
};

#endif // LOGGER_H
