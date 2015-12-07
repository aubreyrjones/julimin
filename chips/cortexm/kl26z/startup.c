//
// Created by ARJ on 12/5/15.
//
// Startup code for Teensy LC, derived from `mk20dx128.c` from teensyduino - Copyright (c) 2013 PJRC.COM, LLC.
//

#include "chip_support.h"
#include <core/startup.h>
#include <string.h>

/** Defined in syscalls.c. */
extern void _start();

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

__attribute__ ((section(".flashconfig"), used))
uint8_t const _flashConfiguration[16] =
		{0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff,
		 0xff, 0xff, 0xff, 0xff, 0xfe, 0xff, 0xff, 0xff };


__attribute__ ((section(".ramnvic"), used))
NVICTable _nvicTable;


void chip_prestart() {
	SIM_COPC = 0;

	// unlock power modes.
	SMC_PMPROT = SMC_PMPROT_AVLP_MASK | SMC_PMPROT_ALLS_MASK | SMC_PMPROT_AVLLS_MASK;

	//SIM_SCGC4 = SIM_SCGC4_USBOTG | 0xF0000030;
	//SIM_SCGC6 = SIM_SCGC6_ADC0 | SIM_SCGC6_TPM0 | SIM_SCGC6_TPM1 | SIM_SCGC6_TPM2 | SIM_SCGC6_FTFL;

	for (int i=0; i < NVIC_NUM_INTERRUPTS; i++) {
		NVIC_SET_PRIORITY(i, 128);
	}
}

void chip_start_core_clocks() {
	// activate capacitors, use external reference crystal (16MHz crystal)
	OSC0_CR = OSC_CR_SC8P_MASK | OSC_CR_SC2P_MASK | OSC_CR_ERCLKEN_MASK;

	// take oscillator reference, set high freq range
	MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_EREFS_MASK;

	// external reference (16MHz), divide by 512
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

	// set USB to use internal clock,
	// take PLL (instead of FLL) reference,
	// output the osc reference clock on RTC_CLKOUT pin (16MHz)
	// UART to use internal clock, TPM to take internal clock
	SIM_SOPT2 = SIM_SOPT2_USBSRC_MASK | SIM_SOPT2_PLLFLLSEL_MASK | SIM_SOPT2_CLKOUTSEL(6)
				| SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_TPMSRC(1);


	// initialize the SysTick counter
//	SYST_RVR = (CPU_CORE_FREQUENCY / 1000) - 1;
//	SYST_CVR = 0;
//	SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
//	SCB_SHPR3 = 0x20200000;  // Systick = priority 32
}

void chip_move_vectors_to_ram() {
	extern char _s_flash_nvic, _s_ram_nvic, _e_ram_nvic;
	memcpy(&_s_ram_nvic, &_s_flash_nvic, &_e_ram_nvic - &_s_ram_nvic);

	SCB_VTOR = (uint32_t) &_s_ram_nvic;
}

void chip_start_status_indicators() {
	// wake from VLLS magic, release IO pins.
	if (PMC_REGSC & PMC_REGSC_ACKISO_MASK) {
		PMC_REGSC |= PMC_REGSC_ACKISO_MASK;
	}

	// set up LED pin
	SIM_SCGC5 |= SIM_SCGC5_PORTC_MASK;
	PORTC_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_DSE_MASK | PORT_PCR_SRE_MASK;
	FGPIOC_PDDR |= (1 << 5);
}