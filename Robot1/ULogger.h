// ULogger.h

#ifndef _ULOGGER_h
#define _ULOGGER_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

enum ULogModes { FULL, NOLCD, NOSERIAL, NONE };

class sTerminal {
protected:
	char ** _buffer;
	uint8_t _height;
	uint8_t _width;
	uint8_t _currentLine;
	void _update();
public:
	sTerminal(uint8_t h, uint8_t w);
	void append(const char * line);
	void status(const char * line);
	void reset();
};

class ULogger {
protected:
	sTerminal _lcd_terminal;
	ULogModes _mode;
public:
	ULogger(uint8_t lcdH, uint8_t lcdW);
	void mode(ULogModes mode);
	void log(const char * data);
	void status(const char * data);
};

#endif

