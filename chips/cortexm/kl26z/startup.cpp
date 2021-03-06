//
// Created by ARJ on 12/5/15.
//
// Startup code for Teensy LC, derived from `mk20dx128.c` from teensyduino - Copyright (c) 2013 PJRC.COM, LLC.
//

#include "chip_support.h"
#include <core/startup.h>
#include <core/syscalls.h>
#include <comm/uart.h>

void _panic(char const* message) {
	nos::console.write(message);

	const static uint32_t SHORT = 600000;
	const static uint32_t LONG = 1000000;
	const uint32_t ledmask = 1 << 5;

	// blink S.O.S
	for (;;) {
		for (int i = 0; i < 3; i++) {
			FGPIOC_PSOR = ledmask;
			spin_delay(SHORT);

			FGPIOC_PCOR = ledmask;
			spin_delay(LONG);
		}

		spin_delay(SHORT);

		for (int i = 0; i < 3; i++){
			FGPIOC_PSOR = ledmask;
			spin_delay(LONG);

			FGPIOC_PCOR = ledmask;
			spin_delay(LONG);

		}

		spin_delay(SHORT);

		for (int i = 0; i < 3; i++) {
			FGPIOC_PSOR = ledmask;
			spin_delay(SHORT);

			FGPIOC_PCOR = ledmask;
			spin_delay(LONG);
		}

		spin_delay(LONG);
		spin_delay(LONG);
		spin_delay(LONG);
	}
}

/** Fault ISR, infinite loop. */
void fault_isr(void) {
	abort();
}

void nmi_isr(void) {
	abort();
}
void unused_isr(void) {
	abort();
}

extern unsigned long _estack;
__attribute__ ((section(".vectors"), used))
NVICTable _vectorTable = {
		&_estack,
		_start,
		nmi_isr,
		fault_isr
};

__attribute__ ((section(".ramnvic"), used))
NVICTable volatile _nvicTable;


__attribute__ ((section(".flashconfig"), used))
uint8_t const _flashConfiguration[16] =
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff };


void chip_prestart() {
	// disable watchdog
	SIM_COPC = 0;

	// unlock power modes.
	SMC_PMPROT = SMC_PMPROT_AVLP_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLLS_MASK;

	for (int i=0; i < NVIC_NUM_INTERRUPTS; i++) {
		NVIC_SET_PRIORITY(i, 128);
	}
}

void chip_start_core_clocks() {
	// activate capacitors, use external reference crystal (16MHz crystal)
	OSC0_CR = OSC_CR_SC8P_MASK | OSC_CR_SC2P_MASK | OSC_CR_ERCLKEN_MASK;

	// the clock is very high freq, take osc1 output
	MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_EREFS_MASK;

	// external reference, divide by 512
	MCG_C1 =  MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);

	// wait for clocks to propagate
	while ((MCG_S & MCG_S_OSCINIT0_MASK) == 0) {};
	while ((MCG_S & MCG_S_IREFST_MASK) != 0) {};
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2)) {};

	// divide external reference by 4
	MCG_C5 = MCG_C5_PRDIV0(3);

	// select PLL as reference, set multiplier to 24: `16 MHz (crystal) / 4 * 24 = 96MHz`
	MCG_C6 = MCG_C6_PLLS_MASK | MCG_C6_VDIV0(0);

	// wait for clock and PLL locks
	while (!(MCG_S & MCG_S_PLLST_MASK)) {};
	while (!(MCG_S & MCG_S_LOCK0_MASK)) {};

	// divide by 2 for main clock: `48MHz`; divide by 2 for bus clock: `48MHz`.
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(1);

	// PLL clock reference (16MHz), divide by 512
	MCG_C1 = MCG_C1_CLKS(0) | MCG_C1_FRDIV(4);

	// wait for clock propagation
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3)) {};

	// set USB to use PLL / 2
	// take PLL (instead of FLL) reference,
	// output the osc reference clock on RTC_CLKOUT pin (16MHz)
	// UART and TPM to to use PLL / 2
	SIM_SOPT2 = SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_CLKOUTSEL(6)
				| SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_TPMSRC(1);

	// wake from VLLS magic, release IO pins.
	if (PMC_REGSC & PMC_REGSC_ACKISO_MASK) {
		PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
	}
}

void chip_move_vectors_to_ram() {
	extern char _s_flash_nvic, _s_ram_nvic, _e_ram_nvic;
	memcpy(&_s_ram_nvic, &_s_flash_nvic, &_e_ram_nvic - &_s_ram_nvic);

	SCB_VTOR = (uint32_t) &_s_ram_nvic;

	__asm__ volatile ("dsb");
}

void chip_start_status_indicators() {

	// set up LED pin
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK | PORT_PCR_SRE_MASK;
	FGPIOC_PDDR |= (1 << 5);

	// set up console pins
	SIM_SCGC5 |= SIM_SCGC5_PORTB_MASK;
	PORTB_PCR16 |= PORT_PCR_MUX(3);
	PORTB_PCR17 |= PORT_PCR_MUX(3);
}
