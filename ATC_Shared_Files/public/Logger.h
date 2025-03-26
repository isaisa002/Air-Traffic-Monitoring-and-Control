#ifndef LOGGER_H
#define LOGGER_H

#include <string>
using namespace std;

class Logger {
public:
    static void logMessage(const string &message);
};

#endif // LOGGER_H
