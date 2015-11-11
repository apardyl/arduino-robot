// utilities.h

#ifndef _UTILITIES_h
#define _UTILITIES_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

extern void resetSource();
extern void sort(uint16_t * tab, uint8_t size);

#endif

