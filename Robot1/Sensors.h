// Sensors.h

#ifndef _SENSORS_h
#define _SENSORS_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#include <LSM303/LSM303.h>
#include <L3G/L3G.h>

class Sonar {
	uint16_t _dist;
	uint8_t _pin;
public:
	void init(uint8_t pin);
	void read();
	uint16_t dist();
};

class MovementSensor {
protected:
	uint8_t pin;
public:
	MovementSensor();
	void init(uint8_t);
	void read();
	uint8_t state;
};

class Encoder {
protected:
	uint32_t _counter;
	uint8_t _state;
	uint8_t _pin;
public:
	void init(uint8_t pin);
	void reset();
	void read();
	uint32_t rotation();
	uint32_t displacement();
};

class Sensors {
 protected:
	 void serialSensor(const char[], int16_t, int16_t, int16_t);
	 void serialSensor(const char[], int32_t);
	 void serialSensor(const char[], float);
 public:
	 L3G l3g;
	 LSM303 lsm303;
	 MovementSensor movement;
	 Encoder encoderL;
	 Encoder encoderR;
	 Sonar sonar;
	 uint16_t compass;

	void init();
	void refresh();
	void fastRefresh();
	void sendSensorStatus();

	LSM303::vector<int16_t> accel();
	LSM303::vector<int16_t> magn();
	L3G::vector<int16_t> gyro();
};

#endif

