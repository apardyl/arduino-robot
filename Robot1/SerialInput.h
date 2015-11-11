// SerialInput.h

#ifndef _SERIALINPUT_h
#define _SERIALINPUT_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

class SerialInput {
 protected:
	 char * _buffer;
	 uint8_t _buffSize;
	 uint8_t _buffEnd;
	 void _process();
	 void _clearBuff();
	 void (*_callback)(const char *);
 public:
	 SerialInput(uint8_t, void(*callback)(const char *));
	 void worker();
	 void reset();
};

#endif

