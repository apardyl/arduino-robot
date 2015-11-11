// 
// 
// 

#include "IOEX.h"

void IOEXinit() {
	expander0.begin(0x20);
	expander0.write(0x00);
}


PCF8574 expander0;