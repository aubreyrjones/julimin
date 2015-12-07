//
// Created by ARJ on 12/4/15.
//

#include <core/led.h>
#include <core/memory_port.h>
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
};