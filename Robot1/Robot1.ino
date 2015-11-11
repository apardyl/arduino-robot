/*
 Name:		Robot1.ino
 Created:	7/27/2015 2:35:02 PM
 Author:	Adam
*/

#include "FreeRoam.h"
#include "ULogger.h"
#include "Servomotor.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <L3G/L3G.h>
#include <LSM303/LSM303.h>
#include <PCF8574/PCF8574.h>
#include <Servo.h>


#include "utilities.h"
#include "Sensors.h"
#include "Motors.h"
#include "Robot.h"
#include "SerialInput.h"
#include "Interpreter.h"
#include "IOEX.h"


void setup() {
	Serial.begin(115200);
	Wire.begin();
	srand(micros());

	robot.init();
}

void loop() {
	robot.worker();

	static volatile uint32_t last;
	static volatile uint32_t counter;
	if (millis() - last >= 30000) {
		char buf[20];
		sprintf(buf, "Cycles/sec: %lu", counter / 30);
		robot.logger.log(buf);
		counter = 0;
		last = millis();
	}
	counter++;

	
}
