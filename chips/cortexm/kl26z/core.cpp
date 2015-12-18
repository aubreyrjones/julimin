//
// Created by ARJ on 12/4/15.
//

#include <core/led.h>
#include "chip_support.h"


namespace nos {
void setStatusLEDState(bool ledLighted) {
	if (ledLighted) {
		FGPIOC_PSOR = (1 << 5);
	}
	else {
		FGPIOC_PCOR = (1 << 5);
	}
}

void toggleStatusLEDState() {
	FGPIOC_PTOR = (1 << 5);
}
};

extern "C" {

__attribute__((used)) bool __atomic_compare_exchange_1(bool *ptr, bool *expected, bool desired, bool weak, int success_memorder, int failure_memorder) {
	__disable_irq();
	if (*ptr == *expected) {
		*ptr = desired;
		__asm__ volatile ("dsb");
		__enable_irq();
	}

	return true;
}

}