#ifndef PRINTD_H
#define PRINTD_H

enum PRINTD_LOGLEVEL {
	LOG_NOTSET = 0,
	LOG_DEBUG  = 1,
	LOG_WARN   = 2,
	LOG_ERROR  = 3,
	LOG_FATAL  = 4
};

void printd(enum PRINTD_LOGLEVEL level, const char* format, ...);

#endif
