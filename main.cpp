#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>
#include <core/errors.h>
#include <analog/DAC.h>
#include "tables/voices.h"

constexpr uint32_t SAMPLE_RATE = 22000;

nos::DAC dac1(DAC0_BASE_PTR);
nos::NCO<uint16_t, 9> osc1(SAMPLE_RATE);


void ncoblink() {
	osc1.step();
	dac1.set(voice1[*osc1]);
}

int main() {
	osc1.setFrequency(440.0f);

	nos::setStatusLEDState(true);

	nos::SystemTimer volatile timer(SAMPLE_RATE, ncoblink);

	for(;;) {};

	return 0;
}

