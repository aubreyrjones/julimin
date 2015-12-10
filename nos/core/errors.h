//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_ERRORS_H
#define JULIMIN_ERRORS_H

#include "startup.h"
#include "led.h"

namespace nos {

/** Panic with a short message. */
inline void panic(char const* message) { _panic(message); }

inline void lockup() { setStatusLEDState(true); while (true) __asm__ volatile ("nop"); }

}

#endif //JULIMIN_ERRORS_H
