// Servomotor.h

#ifndef _SERVOMOTOR_h
#define _SERVOMOTOR_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <Servo.h>

class Servomotor
{
 protected:
	 Servo _servo;
	 uint32_t _lastSet;
	 uint8_t _value;
	 uint8_t _pin;
	 bool _attached;
	 uint32_t _autoRefreshIn;

 public:
	void init(uint8_t pin, uint8_t value, uint32_t autoRefreshIn);
	void set(uint8_t value);
	void worker();
	void refresh();
	void sleep();
	void wakeup();
};

#endif

