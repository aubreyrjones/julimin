//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_SYSTEM_TIMER_H
#define JULIMIN_SYSTEM_TIMER_H

#include <stdint.h>
#include <core/common.h>

namespace nos {

/**
 * This is a timer driven by the SysTick ARM peripheral. It runs at the requested frequency (or near as possible).
 * */
class SystemTimer {
private:
	static SystemTimer *_ins;

protected:

	/** Frequency with which the service routine is called, in Hz. */
	uint32_t frequency = 100;

public:

	/** Create a timer, where `isr` will be called at the given frequency. */
	SystemTimer(uint32_t frequency, VoidFunc isr);

	virtual ~SystemTimer();
};

}

#endif //JULIMIN_SYSTEM_TIMER_H
