//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_SYSCALLS_H
#define JULIMIN_SYSCALLS_H

#include <sys/types.h>
#include <string.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

extern int main() __attribute__ ((section(".text"), used));

/** Startup routine for core. */
void _start();

void abort(void)  __attribute__((noreturn, used));;

/** Stop into an error condition. */
void _abort(void)  __attribute__((noreturn, used));;

/** newlib compliance, always returns 1. */
pid_t _getpid(void) __attribute__((used));

/** newlib compliance, always returns 0. */
pid_t _getppid(void) __attribute__((used));

/** newlib compliance, always generates error. */
int _kill(pid_t pid, int sig) __attribute__((used));

/** Stop running, terminate into a loop. */
void _exit(int status)  __attribute__((noreturn, used));

/** Modify the heap pointer. */
void* _sbrk(ptrdiff_t increment) __attribute__((used));

#ifdef __cplusplus
}
#endif


#endif //JULIMIN_SYSCALLS_H
