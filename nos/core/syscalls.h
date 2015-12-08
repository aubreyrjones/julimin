//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_SYSCALLS_H
#define JULIMIN_SYSCALLS_H

#include <sys/types.h>
#include <string.h>
#include <stddef.h>

extern int main() __attribute__ ((section(".text"), used));

/** Startup routine for core. */
void _start();

void abort(void)  __attribute__((noreturn));;

/** Stop into an error condition. */
void _abort(void)  __attribute__((noreturn));;

/** newlib compliance, always returns 1. */
pid_t _getpid(void);

/** newlib compliance, always returns 0. */
pid_t _getppid(void);

/** newlib compliance, always generates error. */
int _kill(pid_t pid, int sig);

/** Stop running, terminate into a loop. */
void _exit(int status)  __attribute__((noreturn));

/** Modify the heap pointer. */
void* _sbrk(ptrdiff_t increment);

#endif //JULIMIN_SYSCALLS_H
