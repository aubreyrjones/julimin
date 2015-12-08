#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>
#include "tables/voices.h"
#include <core/chip.h>

nos::NCO<uint16_t, 9> volatile osc1(44100, &voice1[0]);

void ncoblink() {
	osc1.step();
	nos::setStatusLEDState(osc1.sample() > 2048);
}

int main() {

	osc1.setFrequency(7.0f);

	nos::SystemTimer volatile timer(44100, ncoblink);

	uint32_t frequency = 7;
	for (; frequency < 40; frequency++){
		nos::spin(1500000);
		osc1.setFrequency(frequency);
	}

	while(true) {};

	return 0;
}
