#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/nco.h>
#include <core/errors.h>
#include <analog/dac.h>
#include <comm/uart.h>
#include "tables/voices.h"

constexpr uint32_t SAMPLE_RATE = 44100;

nos::dac dac1(DAC0_BASE_PTR);
nos::NCO<uint16_t, 9> osc1(SAMPLE_RATE);


void ncoblink() {
	osc1.step();
	dac1.set(voice1[*osc1]);
}

int main() {
	nos::SystemTimer volatile timer(SAMPLE_RATE, ncoblink);
	osc1.setFrequency(440.0f);

	for (;;) {
		nos::console.write("Hello world!\n\r");
		nos::spin(500000);
	}
}

