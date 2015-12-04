//
// Created by ARJ on 12/3/15.
//

#include "syscalls.h"

extern void main();

extern unsigned long _stext;
extern unsigned long _etext;
extern unsigned long _sdata;
extern unsigned long _edata;
extern unsigned long _sbss;
extern unsigned long _ebss;
extern unsigned long _estack;

void fault_isr(void) {
	while (1) { }
}

void reset_handler(void) {
	_start();
}

pid_t _getpid(void) {
	return 1;
}

pid_t _getppid(void) {
	return 0;
}

int _kill(pid_t pid, int sig) {
	return 0;
}

void *_sbrk(ptrdiff_t increment) {
	return 0;
}

void _start() {
	main();
}

void _exit(int status) {
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while (1);
#pragma clang diagnostic pop
}

__attribute__ ((section(".vectors"), used))
void (*const _VectorsFlash[NVIC_NUM_INTERRUPTS + 16])(void) =
		{(void (*)(void)) ((unsigned long) &_estack),
		 reset_handler
};

__attribute__ ((section(".flashconfig"), used))
uint8_t const _FlashConfig[16] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff };