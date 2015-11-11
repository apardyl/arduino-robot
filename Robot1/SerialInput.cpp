// 
// 
// 

#include "SerialInput.h"
#include <Arduino.h>

SerialInput::SerialInput(uint8_t buffSize, void(*callback)(const char *)) {
	_buffSize = buffSize;
	_buffEnd = -1;
	_buffer = new char[buffSize + 1];
	_callback = callback;
}

void SerialInput::_clearBuff() {
	_buffEnd = -1;
}

void SerialInput::reset() {
	_clearBuff();
}

void SerialInput::worker()
{
	while (Serial.available() > 0) {
		char input = Serial.read();
		if (_buffEnd == -1) {
			if (input == '#')
				_buffEnd = 0;
			continue;
		}

		if (input == '\r') {
			continue;
		}
		else if (input == '\n') {
			_process();
			_clearBuff();
		}
		else if (_buffEnd == _buffSize) {
			Serial.println("$$$OVERFLOW!$$$");
			reset();
		}
		else {
			*(_buffer + _buffEnd) = input;
			_buffEnd++;
		}
	}
}

void SerialInput::_process() {
	_buffer[_buffEnd] = '\0';
	_callback(_buffer);
}
