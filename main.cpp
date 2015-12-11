#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/nco.h>
#include <core/errors.h>
#include <analog/dac.h>
#include <comm/uart.h>
#include <comm/i2c.h>
#include <core/errors.h>
#include "tables/voices.h"

// called after everything is set up, after IRQ are enabled, but before the call to main
void main_startup() {
	// port b clock is already enabled for console

	// set up i2c pins: mux 2 (i2c0), pullup enable, pull up (instead of down)
	PORTB_PCR2 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTB_PCR3 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
}

constexpr uint32_t SAMPLE_RATE = 44100;

nos::dac dac1(DAC0_BASE_PTR);
nos::NCO<uint16_t, 9> osc1(SAMPLE_RATE);

void ncoblink() {
	osc1.step();
	dac1.set(voice1[*osc1]);
}

int main() {
	osc1.setFrequency(441.0f);

	nos::SystemTimer volatile timer(SAMPLE_RATE, ncoblink);
	nos::I2CMaster volatile i2c(I2C0_BASE_PTR);

	nos::console.write("Boot: ");
	nos::spin(500000);

	uint8_t ireg;

	for (;;) {
		if (!i2c.readRegisters(0x10, 0, &ireg)) {
			nos::panic("Failed to read.");
		}
		nos::console.writeHex(ireg);
		nos::console.write(". ");
		nos::spin(10000000);
	}
}

