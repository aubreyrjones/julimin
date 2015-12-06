//
// Created by ARJ on 12/6/15.
//

#include <stddef.h>

#ifndef JULIMIN_DELAY_H
#define JULIMIN_DELAY_H

namespace nos {

/**
 * Wastes some time in a loop, spinning `n` times. This is not normalized or synchronized to any reference speed, nor
 * is it guaranteed that `n` loops will equal `n` core clock cycles.
 * */
void spin(size_t n);

}

#endif //JULIMIN_DELAY_H
