//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_CHIP_H
#define JULIMIN_CHIP_H

#include <chip_support.h>

/** In-RAM vector table. */
extern NVICTable _nvicTable;

inline void _mem_barrier() {
	asm volatile ("dmb");
}

inline void _mem_sync() {
	asm volatile ("dsb");
}

#endif //JULIMIN_CHIP_H
