//
// Created by ARJ on 12/3/15.
//

#include <errno.h>
#undef errno

#include "startup.h"
#include "syscalls.h"
#include <stdint.h>

extern int errno;
void* __dso_handle;

void _start() __attribute__((section(".startup")));
void _start()  {
	// do prestart stuff, if anything.
	chip_prestart();

	// start LEDs and debug serial (if present)
	chip_start_status_indicators();

	// set up the clocks
	chip_start_core_clocks();

	// copy data from initializer
	extern char _sdata, _edata, _sdatainit;
	memcpy(&_sdata, &_sdatainit, &_edata - &_sdata);

	// zero bss
	extern char _sbss, _ebss;
	bzero(&_sbss, (&_ebss - &_sbss));

	// let the chip move exception vectors to RAM, if possible.
	chip_move_vectors_to_ram();

	typedef void (*func_ptr) (void);

	// pre-init vector
	extern func_ptr __preinit_array_start, __preinit_array_end;
	for (func_ptr *p = &__preinit_array_start; p != &__preinit_array_end; ++p) {
		(*p)();
	}

	// init vector
	extern func_ptr __init_array_start, __init_array_end;
	for (func_ptr *p = &__init_array_start; p != &__init_array_end; ++p) {
		(*p)();
	}

	// run the user's main, and then "exit".
	_exit(main());
}


void abort(void) {
	_abort();
}

void _abort(void) {
	_panic("ABORT");
}

void*_sbrk(ptrdiff_t increment) {
	extern char _heapStart;
	static char * _heapEnd = 0;

	if (!_heapEnd) {
		_heapEnd = &_heapStart;
	}

	char *prevHeapEnd = _heapEnd;

	_heapEnd += increment;

	return prevHeapEnd;
}

void _exit(int status) {
	while (1) {};
}

pid_t _getpid(void) {
	return 1;
}

pid_t _getppid(void) {
	return 1;
}

int _kill(pid_t pid, int sig) {
	errno = EINVAL;
	return -1;
}
