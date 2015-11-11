// Robot.h

#ifndef _ROBOT_h
#define _ROBOT_h

#include <LiquidCrystal_I2C.h>
#include "SerialInput.h"
#include "Sensors.h"
#include "Motors.h"
#include "Servomotor.h"
#include "ULogger.h"

#define BUZZER 4

class Autopilot {
public:
	void enable();
	void disable();
	void setCourse(int16_t course);
	void worker();
	void stop();
	void setSpeed(uint8_t speed);
	void rotate();
	void start();

	Autopilot();
protected:
	bool _enabled, _move, _rotate;

	int16_t _course;
	int8_t _speed;

};

class Robot {
public:
	SerialInput sinput;
	LiquidCrystal_I2C lcd;
	ULogger logger;
	Servomotor sonarServo;
	Sensors sensors;
	Motors motors;

	//Autopilot autopilot;

	void beep();
	void init();
	void worker();

	enum Modes { NONE, EXTCONTROL, FREEROAM };
	void setMode(Modes mode);

	Robot();

protected:
	Modes _mode;

};

extern Robot robot;

#endif

