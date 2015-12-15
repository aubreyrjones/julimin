#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/nco.h>
#include <core/errors.h>
#include <analog/dac.h>
#include <comm/uart.h>
#include <comm/i2c.h>
#include <stdio.h>
#include "tables/voices.h"

uint32_t constexpr SAMPLE_RATE = 44100;
uint32_t volatile pitchStick = 2048;
bool volatile pitchStickChanged = false;


void pitchStickUpdateISR() {
	uint32_t newVal = ADC0_RA;
	if (newVal != pitchStick) {
		pitchStick = newVal;
		pitchStickChanged = true;
	}
}


// called after everything is set up, after IRQ are enabled, but before the call to main
void main_startup() {
	// port b clock is already enabled for console

	// set up i2c pins: mux 2 (i2c0), pullup enable, pull up (instead of down)
	PORTB_PCR2 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
	PORTB_PCR3 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	SIM_SCGC5 |= SIM_SCGC5_PORTD_MASK;
	SIM_SCGC6 |= SIM_SCGC6_ADC0_MASK;
	ADC0_CFG1 |= ADC_CFG1_ADICLK(0) | ADC_CFG1_ADIV(3) | ADC_CFG1_MODE(1); // bus clock / 4 = 12MHz; 12-bit

	ADC0_CFG2 |= ADC_CFG2_MUXSEL_MASK; // channel b
	ADC0_SC2 |= ADC_SC2_REFSEL(1); //Vcc reference

	_nvicTable.adc = pitchStickUpdateISR;
	_mem_sync();

	NVIC_EnableIRQ(ADC0_IRQn);
}


nos::dac dac1(DAC0_BASE_PTR);
nos::NCO<uint16_t, 9> osc1(SAMPLE_RATE);

auto constexpr lowStop = 30.0f;
auto constexpr highStop = 250.0f;
auto constexpr freqRange = highStop - lowStop;

float fundamentalFrequency = 440.0f;

void tick() {
	osc1.step();
	dac1.set(voice1[*osc1]);

	ADC0_SC1A = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(5); // initiate ADC read on ADC0_SE5b
}

int main() {


	nos::SystemTimer volatile timer(SAMPLE_RATE, tick);

	for (;;) {
		if (pitchStickChanged) {
			fundamentalFrequency = lowStop + ((pitchStick / 4096.0f) * freqRange);
			osc1.setFrequency(fundamentalFrequency);
			pitchStickChanged = false;
		}
	}
}

