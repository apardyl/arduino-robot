#include "Interpreter.h"
#include "Robot.h"
#include <avr/sleep.h>

void Interpreter(const char * command) {
	Serial.println(command);
	if (command[0] == '<') {
		char subbuff[20];
		for (uint8_t i = 0; i < 20; i++) {
			if (command[i + 1] == '\0') { subbuff[i] = '\0';  break; }
			subbuff[i] = command[i + 1];
		}
		robot.logger.log(subbuff);
	}
	if (command[0] == 'F') {
		robot.motors.setEngines(127, 127);
		return;
	}
	if (command[0] == 'D') {
		robot.motors.setEngines(-127, -127);
		return;
	}
	if (command[0] == 'S') {
		set_sleep_mode(SLEEP_MODE_PWR_DOWN);
		sleep_enable();
		sleep_mode();
		return;
	}
	if (command[0] == 'B') {
		robot.motors.motorBreak();
		return;
	}
	if (command[0] == 'L') {
		robot.motors.setEngines(127, 0);
		return;
	}
	if (command[0] == 'R') {
		robot.motors.setEngines(0, 127);
		return;
	}
	if(command[0] == 'K') {
		uint8_t val = ((command[1] - '0') * 100) + ((command[2] - '0') * 10) + ((command[3] - '0'));
		robot.sonarServo.set(val);
		return;
	}
	if(command[0]=='G') {
		robot.setMode(Robot::FREEROAM);
		return;
	}
	
	Serial.println("Unknown command!");
}