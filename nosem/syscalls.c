//
// Created by ARJ on 12/3/15.
//

#include <core.h>

#include <errno.h>
#undef errno
#include "syscalls.h"

extern int errno;

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

void *_sbrk(ptrdiff_t increment) {
	return 0;
}

void _start() {
	main();
	_exit(0);
}
