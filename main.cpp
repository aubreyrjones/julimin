#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>
#include "tables/voices.h"

nos::NCO<uint16_t, 9> osc1(44100, &voice1[0]);

void ncoblink() {
	osc1.step();
	nos::setStatusLEDState(osc1.sample() > 2048);
}

int main() {

	osc1.setFrequency(12);

	nos::SystemTimer volatile timer(44100, ncoblink);

	return 0; // our timer interrupt is still running
}
