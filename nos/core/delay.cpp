//
// Created by ARJ on 12/6/15.
//

#include "delay.h"

namespace nos {

void spin(size_t n) {
	while (n--) __asm__ volatile ("nop");
}

}