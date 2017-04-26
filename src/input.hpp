#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

enum InputBit {

	IFORWARDS = 1,
	ILEFT = 2,
	IBACKWARDS = 4,
	IRIGHT = 8,
	IRETURN = 16
	
};

extern uint8_t input_bits;

#endif
