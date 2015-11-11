#include "Robot.h"
#include "Interpreter.h"
#include "IOEX.h"
#include "utilities.h"
#include "FreeRoam.h"

//#define DEBUG

Robot::Robot() : sinput(32, Interpreter), lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE), logger(4, 20) {
}

void Robot::init()
{	
	pinMode(BUZZER, OUTPUT);

	resetSource();
	_mode = NONE;
	
#ifdef  DEBUG
	while (!Serial) delay(50);
#endif

	logger.mode(NOLCD);
	logger.log("Robot OS 1.0.1");
	logger.log("Serial connection established!");

	lcd.begin(20, 4);
	delay(50);
	lcd.backlight();
	lcd.clear();

	logger.mode(FULL);

	logger.status("uC INIT");
	logger.log("uC init...");
	logger.log("Init subsystems:");
	pinMode(LED_BUILTIN, OUTPUT);
	digitalWrite(LED_BUILTIN, HIGH);

	logger.log("Init: LCD...");
	logger.log("OK!");

	logger.log("Init: IOEx... ");
	IOEXinit();
	logger.log("OK!");

	logger.log("Init: sensors... ");
	sensors.init();
	logger.log("OK!");

	logger.log("Init: motors... ");
	motors.init();
	logger.log("OK!");

	logger.log("Init: servos... ");
	sonarServo.init(9, 90, 10000);
	logger.log("OK!");

	logger.log("Init completed!");
	logger.status("uC Ready!");

	beep();
	delay(500);

	setMode(NONE);

	//autopilot.setSpeed(63);
	//autopilot.setCourse(0);
	//autopilot.rotate();
	//autopilot.start();
	//autopilot.enable();

	//setMode(EXTCONTROL);
	setMode(FREEROAM);

	//motors.setEngines(-63, 63);
	
}

void Robot::worker() {
	sinput.worker();
	//sensors.fastRefresh();

	static uint32_t slowLoop;
	if (millis() - slowLoop >= 50) { //20Hz
		slowLoop = millis();
		//sensors.refresh();

		if (_mode == EXTCONTROL) {
			//sensors.sendSensorStatus();
		}

		//autopilot.worker();
		sonarServo.worker();
	}
	if (_mode == FREEROAM) {
		freeRoam();
	}
}

void Robot::setMode(Modes mode) {
	_mode = mode;
	switch (mode) {
		case NONE:
			robot.logger.status("READY");
			break;
		case FREEROAM: 
			robot.logger.status("FREEROAM");
			break;
		case EXTCONTROL:
			robot.logger.status("Ext CONTROL");
			break;
		default: break;
	}
}

void Robot::beep() {
	tone(BUZZER, 523, 200);
	delay(50);
	tone(BUZZER, 659, 200);
}


Robot robot;

void Autopilot::enable() {
	_enabled = true;
}

void Autopilot::disable() {
	_enabled = false;
}

void Autopilot::setCourse(int16_t course) {
	_course = course;
}

void Autopilot::worker() {
	if (_enabled) {
		if ((!_move) && (!_rotate)) {
			robot.motors.setEngines(0, 0);
			return;
		}

		static int16_t ddif;

		Serial.println(robot.sensors.compass);

		ddif = (robot.sensors.compass + ((robot.sensors.compass < _course) ? 360 : 0) - _course);
		Serial.println(ddif);
		ddif = (ddif > 180) ? -(360 - ddif) : ddif;

		Serial.println(ddif);

		if (_rotate) {
			if (ddif < -25) {
				robot.motors.setEngines(_speed, -_speed);
			}
			else if (ddif < -10) {
				robot.motors.setEngines(_speed / 2, -_speed / 2);
			}
			else if (ddif > 25) {
				robot.motors.setEngines(-_speed, _speed);
			}
			else if (ddif > 10) {
				robot.motors.setEngines(-_speed / 2, _speed / 2);
			}
			else {
				_rotate = false;
			}
		}

		if (_move && !_rotate) {
			if (ddif < -1) {
				robot.motors.setEngines(_speed, _speed - (2 * (_speed * abs(ddif) / 180)));
			}
			else if (ddif > 1) {
				robot.motors.setEngines(_speed - (2 * (_speed * abs(ddif) / 180)), _speed);
			}
		}
	}
}

void Autopilot::stop() {
	_move = false;
	_rotate = false;
}

void Autopilot::setSpeed(uint8_t speed) {
	_speed = speed;
}

void Autopilot::rotate() {
	_rotate = true;
}

void Autopilot::start() {
	_move = true;
}

Autopilot::Autopilot() : _enabled(false), _move(false), _rotate(false), _course(0), _speed(127) {
}