// 
// 
// 

#include "Motors.h"
#include "IOEX.h"

void Motors::init()
{
	_motorR.init(6, 0, 1, 1);
	_motorL.init(11, 3, 2, 1);
}

void Motors::setEngines(int8_t left, int8_t right) {
	if (left != _spL) {
		_spL = left;
		_motorL.setMotor(left);
	}
	if (right != _spR) {
		_spR = right;
		_motorR.setMotor(right);
	}
}

void Motors::motorBreak() {
	_motorL.motorBreak();
	_motorR.motorBreak();
}

void Motor::setMotor(int8_t speed) {
	if (speed == 0) {
		expander0.digitalWrite(_in1, HIGH);
		expander0.digitalWrite(_in2, HIGH);
		digitalWrite(_pwm, LOW);
		return;
	}
	bool reverse = (speed < 0);
	speed = uint8_t(float(abs(speed)) * _offset);
	if (!reverse) {
		expander0.digitalWrite(_in1, HIGH);
		expander0.digitalWrite(_in2, LOW);
		analogWrite(_pwm, speed << 1);
	}
	else if (reverse) {
		expander0.digitalWrite(_in1, LOW);
		expander0.digitalWrite(_in2, HIGH);
		analogWrite(_pwm, speed << 1);
	}
}

void Motor::init(uint8_t pwm, uint8_t EXin1, uint8_t EXin2, float offset) {
	_in1 = EXin1;
	_in2 = EXin2;
	_pwm = pwm;
	_offset = offset;

	expander0.pinMode(_in1, OUTPUT);
	expander0.pinMode(_in2, OUTPUT);
	pinMode(_pwm, OUTPUT);
	setMotor(0);
}

void Motor::motorBreak() {
	expander0.digitalWrite(_in1, HIGH);
	expander0.digitalWrite(_in2, HIGH);
	digitalWrite(_pwm, HIGH);
}