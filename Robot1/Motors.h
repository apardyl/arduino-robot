// Motors.h

#ifndef _MOTORS_h
#define _MOTORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <PCF8574/PCF8574.h>

class Motor {
	uint8_t _pwm;
	uint8_t _in1;
	uint8_t _in2;
	float _offset;
public:
	void init(uint8_t pwm, uint8_t EXin1, uint8_t EXin2, float offset);
	void setMotor(int8_t speed);
	void motorBreak();
};

class Motors
{
 protected:
	 Motor _motorL;
	 Motor _motorR;
	 int8_t _spL, _spR;

 public:
	void init();
	void motorBreak();
	void setEngines(int8_t left, int8_t right);

};

#endif

