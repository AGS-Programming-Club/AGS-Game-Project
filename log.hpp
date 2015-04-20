#ifndef LOG_H
#define LOG_H

#include <string>

enum Severity {INFO, WARN, ERROR};

void log(Severity level, std::string message);

#endif
