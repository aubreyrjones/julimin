//
// Created by ARJ on 12/3/15.
//

#include "syscalls.h"

pid_t _getpid(void) {
	return 1;
}

pid_t _getppid(void) {
	return 0;
}

int _kill(pid_t pid, int sig) {
	return 0;
}

void* _sbrk(ptrdiff_t increment) {
	return 0;
}

void _exit(int status) {
	while(1);
}