//
// Created by ARJ on 12/5/15.
//
// Startup code for Teensy LC, derived from `mk20dx128.c` from teensyduino - Copyright (c) 2013 PJRC.COM, LLC.
//

#include "startup.h"
#include "kinetis.h"


void cstart_prestart() {
	// wake from VLLS magic, release IO pins.
	if (PMC_REGSC & PMC_REGSC_ACKISO) {
		PMC_REGSC |= PMC_REGSC_ACKISO;
	}

	// unlock power modes.
	SMC_PMPROT = SMC_PMPROT_AVLP | SMC_PMPROT_ALLS | SMC_PMPROT_AVLLS;

	for (int i=0; i < NVIC_NUM_INTERRUPTS; i++) NVIC_SET_PRIORITY(i, 128);
}

void cstart_core_clocks() {
	MCG_C1 = MCG_C1_CLKS(1) | MCG_C1_IREFS | MCG_C1_IRCLKEN;
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(1)) ;

	// activate capacitors, use external reference crystal (16MHz crystal)
	OSC0_CR = OSC_SC8P | OSC_SC2P | OSC_ERCLKEN;

	// take oscillator reference, set high freq range
	MCG_C2 = MCG_C2_RANGE0(2) | MCG_C2_EREFS;

	// external reference (16MHz), divide by 512
	MCG_C1 =  MCG_C1_CLKS(2) | MCG_C1_FRDIV(4);

	// wait for clocks to propagate
	while ((MCG_S & MCG_S_OSCINIT0) == 0);
	while ((MCG_S & MCG_S_IREFST) != 0);
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(2));

	// divide external reference by 4
	MCG_C5 = MCG_C5_PRDIV0(3);

	// select PLL as reference, set multiplier to 24: `16 MHz (crystal) / 4 * 24 = 96MHz`
	MCG_C6 = MCG_C6_PLLS | MCG_C6_VDIV0(0);

	// wait for clock and PLL locks
	while (!(MCG_S & MCG_S_PLLST)) ;
	while (!(MCG_S & MCG_S_LOCK0)) ;

	// divide by 2 for main clock: `48MHz`; divide by 2 for bus clock: `48MHz`.
	SIM_CLKDIV1 = SIM_CLKDIV1_OUTDIV1(1) | SIM_CLKDIV1_OUTDIV4(1);

	// PLL clock reference (16MHz), divide by 512
	MCG_C1 = MCG_C1_CLKS(0) | MCG_C1_FRDIV(4);

	// wait for clock propagation
	while ((MCG_S & MCG_S_CLKST_MASK) != MCG_S_CLKST(3)) ;

	// set USB to use internal clock,
	// take PLL (instead of FLL) reference,
	// output the osc reference clock on RTC_CLKOUT pin (16MHz)
	// UART to use internal clock, TPM to take internal clock
	SIM_SOPT2 = SIM_SOPT2_USBSRC | SIM_SOPT2_PLLFLLSEL | SIM_SOPT2_CLKOUTSEL(6)
				| SIM_SOPT2_UART0SRC(1) | SIM_SOPT2_TPMSRC(1);

	// initialize the SysTick counter
	SYST_RVR = (CPU_CORE_FREQUENCY / 1000) - 1;
	SYST_CVR = 0;
	SYST_CSR = SYST_CSR_CLKSOURCE | SYST_CSR_TICKINT | SYST_CSR_ENABLE;
	SCB_SHPR3 = 0x20200000;  // Systick = priority 32


//	SIM_SCGC4 = SIM_SCGC4_USBOTG | 0xF0000030; // these are periph clocks
//	SIM_SCGC5 = 0x00003F82;		// clocks active to all GPIO
//	SIM_SCGC6 = SIM_SCGC6_ADC0 | SIM_SCGC6_TPM0 | SIM_SCGC6_TPM1 | SIM_SCGC6_TPM2 | SIM_SCGC6_FTFL;
}