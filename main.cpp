#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>
#include "tables/voices.h"

constexpr uint32_t SAMPLE_RATE = 22000;

nos::NCO<uint16_t, 9> volatile osc1(SAMPLE_RATE);
nos::NCO<uint16_t, 9> volatile osc2(SAMPLE_RATE);

void ncoblink() {
	osc1.step();
	osc2.step();
	nos::setStatusLEDState(voice1[osc1.i()] > 2048);
}

int main() {

	osc1.setFrequency(2.5f);

	nos::SystemTimer volatile timer(SAMPLE_RATE, ncoblink);


	uint32_t frequency = 7;
	for (; frequency < 40; frequency++){
		nos::spin(1500000);
		osc1.setFrequency(frequency);
	}

	while(true) {};

	return 0;
}

