// 
// 
// 

#include "utilities.h"
#include "Robot.h"

void sort(uint16_t * tab, uint8_t size) {
	bool done = true;
	do {
		for (uint8_t i = 0; i < size;i++) {
			if (*(tab + i) > *(tab + i + 1)) {
				done = false;
				uint16_t tmp = *(tab + i);
				*(tab + i) = *(tab + i + 1);
				*(tab + i + 1) = tmp;
			}
		}
		size--;
		if (done) return;
		done = true;
	} while (size > 1);
}

void beep(uint8_t number) {
	while(1) {
		digitalWrite(BUZZER, HIGH);
		delay(100);
		digitalWrite(BUZZER, LOW);
		if (number <= 1) break;
		number--;
		delay(200);
	}
}

void resetSource() {
	if (MCUSR & 1 << PORF) beep(1);
	if (MCUSR & 1 << BORF) beep(2);
	if (MCUSR & 1 << EXTRF) beep(3);
	if (MCUSR & 1 << WDRF) beep(4);
	if (MCUSR & 1 << JTRF) beep(5);
	
	MCUSR = 0;
}