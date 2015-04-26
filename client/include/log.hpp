#ifndef LOG_H
#define LOG_H

#include <string>

enum Severity {INFO, WARN, ERROR};

/** Logs a message
 *
 * 	@param level The importance of the message, info is just bacis information
 * 	warn is something that needs to be addressed but need not crash the game
 * 	error is something catastrofic that the game should not recover from.
 **/
void log(Severity level, std::string message);

#endif
