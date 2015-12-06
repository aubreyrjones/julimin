//
// Created by ARJ on 12/4/15.
//

#include "core.h"
extern unsigned long _estack;

void fault_isr(void) {
	while (1) { }
}

void reset_handler(void) {
	_start();
}

__attribute__ ((section(".stackstart"), used))
const void * _stackStart [1] = {
		&_estack
};

__attribute__ ((section(".vectors"), used))
void (*const _vectorTable[NVIC_NUM_INTERRUPTS + 16])(void) = {
		reset_handler,
		reset_handler,
		fault_isr,
		fault_isr,
		fault_isr,
		fault_isr
};

__attribute__ ((section(".flashconfig"), used))
uint8_t const _flashConfiguration[16] =
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff };
