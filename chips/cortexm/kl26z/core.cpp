//
// Created by ARJ on 12/4/15.
//

#include <core/led.h>
#include <core/errors.h>
#include <core/delay.h>
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

void panic(char const* message) {
	// TODO: serial debugging

	constexpr size_t n = 50000;

	setStatusLEDState(true);
	spin(n);
	setStatusLEDState(false);
	spin(3 * n);
}

};