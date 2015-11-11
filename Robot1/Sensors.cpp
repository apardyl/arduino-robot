// 
// 
// 

#include "Sensors.h"

void Sensors::serialSensor(const char name[], int16_t x, int16_t y, int16_t z) {
	Serial.print('$');
	Serial.print(name);
	Serial.print(',');
	Serial.print(x);
	Serial.print(',');
	Serial.print(y);
	Serial.print(',');
	Serial.print(z);
	Serial.print("\r\n");
}

void Sensors::serialSensor(const char name[], int32_t a) {
	Serial.print('$');
	Serial.print(name);
	Serial.print(',');
	Serial.print(a);
	Serial.print("\r\n");
}

void Sensors::serialSensor(const char name[], float a) {
	Serial.print('$');
	Serial.print(name);
	Serial.print(',');
	Serial.print(a);
	Serial.print("\r\n");
}

void Sensors::init() {
	if (!lsm303.init()) {
		while (true) {}
	}
	if (!l3g.init()) {
		while (true) {}
	}
	lsm303.enableDefault();
	l3g.enableDefault();

	//movement.init(12);
	encoderL.init(7);
	encoderR.init(8);
	sonar.init(18);

	refresh();
}

void Sensors::refresh() {
	lsm303.read();
	l3g.read();
	//sonar.read();
	compass = uint16_t(lsm303.heading());
	//movement.read();
}

void Sensors::fastRefresh() {
	encoderL.read();
	encoderR.read();
}

void Sensors::sendSensorStatus() {
	serialSensor("accel", lsm303.a.x, lsm303.a.y, lsm303.a.z);
	serialSensor("magn", lsm303.m.x, lsm303.m.y, lsm303.m.z);
	serialSensor("gyro", l3g.g.x, l3g.g.y, l3g.g.z);
	//serialSensor("move", movement.state);
	serialSensor("encL", int32_t(encoderL.rotation()));
	serialSensor("encR", int32_t(encoderR.rotation()));
	serialSensor("dist", int32_t(sonar.dist()));
	serialSensor("head", lsm303.heading());
}

LSM303::vector<int16_t> Sensors::accel() {
	return lsm303.a;
}

LSM303::vector<int16_t> Sensors::magn() {
	return lsm303.m;
}

L3G::vector<int16_t> Sensors::gyro() {
	return l3g.g;
}

MovementSensor::MovementSensor() {
	state = 0;
}


void MovementSensor::init(uint8_t Pin) {
	pin = Pin;
	pinMode(pin, INPUT);
}

void MovementSensor::read() {
	state = digitalRead(pin);
}

void Encoder::init(uint8_t pin) {
	reset();
	_pin = pin;
	pinMode(pin, INPUT);
	_state = digitalRead(_pin);
}

void Encoder::reset() {
	_counter = 0;
	_state = 0;
}

void Encoder::read() {
	uint8_t newState = digitalRead(_pin);
	if (newState != _state) {
		_state = newState;
		_counter++;
	}
}

uint32_t Encoder::rotation() {
	return _counter;
}

uint32_t Encoder::displacement() {
	return _counter / 20;
}

void Sonar::init(uint8_t pin) {
	pinMode(pin, INPUT);
	_pin = pin;
}

void Sonar::read() {
	_dist = pulseIn(_pin, LOW) /50;
	if (_dist > 500) _dist = 500;
}

uint16_t Sonar::dist() {
	return _dist;
}
