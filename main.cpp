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
uint32_t volatile inputAxes[2] = {2048, 2048};
bool volatile pitchStickChanged = false;


void pitchStickUpdateISR() {
	uint32_t newVal = ADC0_RA;

	if (newVal != inputAxes[0]) {
		inputAxes[0] = newVal;
		pitchStickChanged = true;
	}
}


// called after everything is set up, after IRQ are enabled, but before the call to main
void main_startup() {
	// port b clock is already enabled for console

	// set up i2c pins: mux 2 (i2c0), pullup enable, pull up (instead of down)
//	PORTB_PCR2 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;
//	PORTB_PCR3 |= PORT_PCR_MUX(2) | PORT_PCR_PE_MASK | PORT_PCR_PS_MASK;

	// set up stick ADCs
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
nos::NCO<uint16_t, 9> volatile osc1(SAMPLE_RATE);

nos::fixed const lowStop = 30.0_f;
nos::fixed const highStop = 250.0_f;
nos::fixed const freqRange = highStop - lowStop;
nos::fixed const halfRange = freqRange / 2.0_f;

nos::fixed const centerPitch = lowStop + halfRange;

nos::fixed fundamentalFrequency = 440.0_f;
//nos::fixed beatFrequency = fundamentalFrequency + 5.0_f;

void tick() {
	osc1.step();

	dac1.set(voice1[*osc1]);

	ADC0_SC1A = ADC_SC1_AIEN_MASK | ADC_SC1_ADCH(5); // initiate ADC read on ADC0_SE5b
}

int main() {
	osc1.setFrequency(fundamentalFrequency);

	nos::SystemTimer volatile timer(SAMPLE_RATE, tick);

	for (;;) {
		if (pitchStickChanged) {
			nos::fixed pitchStick(nos::fixed((int32_t) inputAxes[0]) / 4096.0_f - 0.5_f);

			fundamentalFrequency = centerPitch + (pitchStick * halfRange);
			osc1.setFrequency(fundamentalFrequency);
			pitchStickChanged = false;
		}
	}
}

