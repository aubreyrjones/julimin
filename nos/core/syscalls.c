//
// Created by ARJ on 12/3/15.
//

#include <core.h>

#include <errno.h>
#undef errno
#include "syscalls.h"

extern int errno;
static char * _heapEnd = 0;

void _start() {
	// copy data from initializer
	extern char _sdata, _edata, _sdatainit;
	memcpy(&_sdata, &_sdatainit, &_edata - &_sdata);

	// zero bss
	extern unsigned long _sbss, _ebss;
	bzero(&_sbss, (&_ebss - &_sbss) * sizeof(unsigned long));

	_exit(main());
}

void abort(void) {
	_exit(0);
}

void _abort(void) {
	_exit(0);
}

void*_sbrk(ptrdiff_t increment) {
	extern char _heapStart;

	if (!_heapEnd) {
		_heapEnd = &_heapStart;
	}

	char *prevHeapEnd = _heapEnd;

	_heapEnd += increment;

	return prevHeapEnd;
}

void _exit(int status) {
	while (1);
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
