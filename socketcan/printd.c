#include "printd.h"


/*
 Colour     f;b         Colour          f;b

 Black      0;30        Dark Gray       1;30
 Blue       0;34        Light Blue      1;34
 Green      0;32        Light Green     1;32
 Cyan       0;36        Light Cyan      1;36
 Red        0;31        Light Red       1;31
 Purple     0;35        Light Purple    1;35
 Brown      0;33        Yellow          1;33
 Light Gray 0;37        White           1;37
*/


#include <stdarg.h>
#include <stdio.h>

void printd(enum PRINTD_LOGLEVEL level, const char* format, ...) {
	const char* color = "\e[%d;%dm";
	
	// Prefix
	printf(color, 1, 30);
	printf("lib-debug: ");

	// Color
	switch(level) {
		case(LOG_FATAL): printf(color, 1, 31); break;
		case(LOG_ERROR): printf(color, 0, 31); break;
		case(LOG_WARN):  printf(color, 1, 37); break;
		default: printf(color, 0, 37 ); break;
	}

	
	va_list args;
	va_start(args, format);
	vprintf(format, args);
	va_end(args);

	// Reset color
	printf("\e[%d;%dm", 0, 0);
	fflush(stdout);
}

