//
// Created by ARJ on 12/6/15.
//

#include <core/chip.h>
#include <core/errors.h>
#include <kl26z/chip_support.h>
#include "SystemTimer.h"

namespace nos {

SystemTimer* SystemTimer::_ins = nullptr;

SystemTimer::SystemTimer(uint32_t frequency, VoidFunc isr) {
	if (_ins) { // there's already an instance
		panic("Timer Exists");
	}

	_ins = this;

	uint32_t ticksIn1s;

	uint32_t ticksIn10ms = SYSTICK_CALIB;
	if (!ticksIn10ms) {
		ticksIn1s = CPU_CORE_FREQUENCY;
	}
	else {
		ticksIn1s = ticksIn10ms * 100;
	}

	uint32_t ticksInPeriod = (ticksIn1s / frequency);
	if (ticksInPeriod) ticksInPeriod--; // off-by-one according to manual, `if` checks underflow.
	SYSTICK_RVR = ticksInPeriod;

	_nvicTable.sysTick = isr;
}

SystemTimer::~SystemTimer() {


	if (_ins == this) {
		_ins = nullptr;
	}
}

}