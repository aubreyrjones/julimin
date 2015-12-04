//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_SYSCALLS_H
#define JULIMIN_SYSCALLS_H

#include <sys/types.h>
#include <stddef.h>

extern void main() __attribute__ ((section(".text"), used));


//pid_t _getpid(void);
//
//pid_t _getppid(void);
//
//int _kill(pid_t pid, int sig);

void _start();

void _exit(int status)  __attribute__((noreturn));

void* _sbrk(ptrdiff_t increment);

#endif //JULIMIN_SYSCALLS_H
