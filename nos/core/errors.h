//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_ERRORS_H
#define JULIMIN_ERRORS_H

#include "startup.h"

namespace nos {

/** Panic with a short message. */
inline void panic(char const* message) { _panic(message); }

}

#endif //JULIMIN_ERRORS_H
