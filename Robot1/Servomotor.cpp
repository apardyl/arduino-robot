#include "Servomotor.h"

void Servomotor::init(uint8_t pin, uint8_t value, uint32_t autoRefreshIn)
{
	_pin = pin;
	_value = value;
	_autoRefreshIn = autoRefreshIn;
	_attached = false;
	_lastSet = 0;

	refresh();
}

void Servomotor::set(uint8_t value) {
	_value = value;
	
	refresh();
}

void Servomotor::worker() {
	if (_autoRefreshIn != 0 && _autoRefreshIn < 100) {
		return;
	}
	if (millis() - _lastSet > 100 && _attached) {
		sleep();
	}
	if (_autoRefreshIn != 0 && millis() - _lastSet > _autoRefreshIn) {
		refresh();
	}
}

void ::Servomotor::refresh() {
	if (!_attached) {
		wakeup();
	}

	_lastSet = millis();
	_servo.write(_value);
}

void Servomotor::sleep() {
	_servo.detach();
	_attached = false;
}

void Servomotor::wakeup() {
	_servo.attach(_pin);
	_attached = true;
}