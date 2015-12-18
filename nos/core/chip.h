//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_CHIP_H
#define JULIMIN_CHIP_H

#include <chip_support.h>

#ifdef __cplusplus
extern "C" {
#endif

/** In-RAM vector table. */
extern NVICTable volatile _nvicTable;

inline void _mem_barrier() {
	__asm__ volatile ("dmb");
}

inline void _mem_sync() {
	__asm__ volatile ("dsb");
}

extern int errno;
extern void* __dso_handle;

#ifdef __cplusplus
}
#endif


namespace nos {

struct IRQDisableRAII {
	IRQDisableRAII() {
		__disable_irq();
	}

	void unlock() volatile {
		__enable_irq();
	}

	~IRQDisableRAII() {
		unlock();
	}
};

#define NOS_NOIRQ ::nos::IRQDisableRAII volatile __irqDisableRAII;

}


#endif //JULIMIN_CHIP_H
