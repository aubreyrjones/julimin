//
// Created by ARJ on 12/6/15.
//

#include <core/chip.h>
#include <core/errors.h>
#include "SystemTimer.h"

namespace nos {

SystemTimer* SystemTimer::_ins = nullptr;

SystemTimer::SystemTimer(uint32_t frequency, VoidFunc isr) {
	if (_ins) { // there's already an instance
		panic("Timer Exists");
	}

	_ins = this;

	uint32_t ticksIn10ms = SYSTICK_CALIB;
	uint32_t ticksInPeriod;
	if (!ticksIn10ms) {
		ticksInPeriod = CPU_CORE_FREQUENCY / frequency;
	}
	else {
		ticksInPeriod = (ticksIn10ms * 100) / frequency;
	}


	if (ticksInPeriod) ticksInPeriod--; // manual-suggested adjustment, `if` checks underflow.
	SYSTICK_RVR = ticksInPeriod & 0xffffff;

	_nvicTable.sysTick = isr;

	_mem_barrier();

	SYSTICK_CSR |= SYSTICK_CSR_CLKSOURCE | SYSTICK_CSR_TICKINT | SYSTICK_CSR_ENABLE;
}

SystemTimer::~SystemTimer() {
	SYSTICK_CSR |= ~SYSTICK_CSR_ENABLE;

	if (_ins == this) {
		_ins = nullptr;
	}
}

}