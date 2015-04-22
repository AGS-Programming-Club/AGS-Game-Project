#include <iostream>
#include <string>
#include "log.hpp"

using namespace std;

/* At the moment this only prints the message, logging levels
 * and other features can be added nature
 */
void log(Severity level, string message) {
	switch(level) {
		case ERROR:
			cout << "[ERROR]" << message << endl;
			break;
		case WARN:
			cout << "[WARN]" << message << endl;
			break;
		case INFO:
			cout << "[INFO]" << message << endl;
			break;
	}
}
