// 
// 
// 
#include "Robot.h"

#include "ULogger.h"

void sTerminal::_update() {
	for (int i = 0; i < _height; i++) {
		robot.lcd.setCursor(0, _height - i);
		robot.lcd.print(_buffer[(_currentLine + i) % _height]);
	}
}

sTerminal::sTerminal(uint8_t h, uint8_t w) {
	h--;
	_height = h;
	_width = w;
	_buffer = new char *[_height];
	for (uint8_t i = 0; i < h;i++) {
		_buffer[i] = new char[w + 1];
		for (uint8_t j = 0; j <= w; j++) {
			_buffer[i][j] = '\0';
		}
	}

	reset();
}

void sTerminal::append(const char* line) {
	_currentLine = (_currentLine + _height - 1) % _height;
	uint8_t i = 0;
	while (i<_width) {
		if (line[i] != '\0') {
			_buffer[_currentLine][i] = line[i];
			i++;
		}
		else break;
	}
	for (; i < _width; i++) {
		_buffer[_currentLine][i] = ' ';
	}

	_update();
}

void sTerminal::status(const char* line) {
	uint8_t lenght = 0;
	while (line[lenght] != '\0' && line[lenght]!='\n' && lenght < _width) lenght++;

	robot.lcd.setCursor((_width - lenght) / 2, 0);
	robot.lcd.print(line);
}

void sTerminal::reset() {
	for (int i = 0; i < _height; i++) {
		for (int j = 0; j < _width; j++) {
			_buffer[i][j] = '\0';
		}
	}

	robot.lcd.clear();
}

ULogger::ULogger(uint8_t lcdH, uint8_t lcdW) : _lcd_terminal(lcdH, lcdW) {
	_mode = NONE;
}

void ULogger::mode(ULogModes mode) {
	_mode = mode;
}

void ULogger::log(const char * data) {
	if (_mode == FULL || _mode == NOLCD) {
		Serial.print(">");
		Serial.print(data);
		Serial.print("\r\n");
	}
	if (_mode == FULL || _mode == NOSERIAL) {
		_lcd_terminal.append(data);
	}
}

void ULogger::status(const char* data) {
	if (_mode == FULL || _mode == NOLCD) {
		Serial.print(">>>");
		Serial.print(data);
		Serial.print("\r\n");
	}
	if (_mode == FULL || _mode == NOSERIAL) {
		_lcd_terminal.status(data);
	}
}