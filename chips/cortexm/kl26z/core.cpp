//
// Created by ARJ on 12/4/15.
//

#include <core/led.h>
#include "kinetis.h"

namespace nos {
void setStatusLEDState(bool ledLighted) {
	if (ledLighted) {
		GPIOC_PSOR = (1 << 5);
	}
	else {
		GPIOC_PCOR = (1 << 5);
	}
}
};