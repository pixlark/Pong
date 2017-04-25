#ifndef INPUT_H
#define INPUT_H

#include <stdint.h>

enum InputBit {

	IFORWARDS = 1,
	ILEFT = 2,
	IBACKWARDS = 4,
	IRIGHT = 8,
	IACTION1 = 16,
	IACTION2 = 32,
	IACTION3 = 64,
	IACTION4 = 128
	
};

extern uint8_t input_bits;

#endif
