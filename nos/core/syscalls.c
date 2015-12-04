//
// Created by ARJ on 12/3/15.
//

#include <core.h>

#include <errno.h>
#undef errno
#include "syscalls.h"

extern int errno;
static char * _heapEnd = 0;

void* memcpy(void* dest, void* src, size_t n) {
	char *d = (char*) dest;
	char *s = (char*) src;
	for (; n != 0; n--) {
		d[n - 1] = s[n - 1];
	}

	return dest;
}

void abort(void) {
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
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
	while (1);
#pragma clang diagnostic pop
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

void _start() {
	main();
	_exit(0);
}
