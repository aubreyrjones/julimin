//
// Created by ARJ on 12/6/15.
//

#ifndef JULIMIN_CHIP_SUPPORT_H
#define JULIMIN_CHIP_SUPPORT_H

#include "MKL26Z4.h"
#include "MKL26Z4_extension.h"
#include "MKL26Z4_features.h"

#define SCB_CPUID		(*(const    uint32_t *)0xE000ED00) // CPUID Base Register
#define SCB_ICSR		(*(volatile uint32_t *)0xE000ED04) // Interrupt Control and State
#define SCB_ICSR_PENDSTSET		((uint32_t)0x04000000)
#define SCB_VTOR		(*(volatile uint32_t *)0xE000ED08) // Vector Table Offset
#define SCB_AIRCR		(*(volatile uint32_t *)0xE000ED0C) // Application Interrupt and Reset Control
#define SCB_SCR			(*(volatile uint32_t *)0xE000ED10) // System Control Register
#define SCB_CCR			(*(volatile uint32_t *)0xE000ED14) // Configuration and Control
#define SCB_SHPR1		(*(volatile uint32_t *)0xE000ED18) // System Handler Priority Register 1
#define SCB_SHPR2		(*(volatile uint32_t *)0xE000ED1C) // System Handler Priority Register 2
#define SCB_SHPR3		(*(volatile uint32_t *)0xE000ED20) // System Handler Priority Register 3
#define SCB_SHCSR		(*(volatile uint32_t *)0xE000ED24) // System Handler Control and State
#define SCB_CFSR		(*(volatile uint32_t *)0xE000ED28) // Configurable Fault Status Register
#define SCB_HFSR		(*(volatile uint32_t *)0xE000ED2C) // HardFault Status
#define SCB_DFSR		(*(volatile uint32_t *)0xE000ED30) // Debug Fault Status
#define SCB_MMFAR		(*(volatile uint32_t *)0xE000ED34) // MemManage Fault Address

#define SYSTICK_CSR		(*(volatile uint32_t *)0xE000E010) // SysTick Control and Status
#define SYSTICK_CSR_COUNTFLAG		((uint32_t)0x00010000)
#define SYSTICK_CSR_CLKSOURCE		((uint32_t)0x00000004)
#define SYSTICK_CSR_TICKINT		((uint32_t)0x00000002)
#define SYSTICK_CSR_ENABLE			((uint32_t)0x00000001)

#define SYSTICK_RVR		(*(volatile uint32_t *)0xE000E014) // SysTick Reload Value Register
#define SYSTICK_CVR		(*(volatile uint32_t *)0xE000E018) // SysTick Current Value Register
#define SYSTICK_CALIB		(*(const    uint32_t *)0xE000E01C) // SysTick Calibration Value


#define ARM_DEMCR		(*(volatile uint32_t *)0xE000EDFC) // Debug Exception and Monitor Control
#define ARM_DEMCR_TRCENA		(1 << 24)	 // Enable debugging & monitoring blocks
#define ARM_DWT_CTRL		(*(volatile uint32_t *)0xE0001000) // DWT control register
#define ARM_DWT_CTRL_CYCCNTENA		(1 << 0)		// Enable cycle count
#define ARM_DWT_CYCCNT		(*(volatile uint32_t *)0xE0001004) // Cycle count register



#define CPU_CORE_FREQUENCY 48000000
#define NVIC_NUM_INTERRUPTS 32
#define NVIC_SET_PRIORITY(irqnum, priority) (*((uint32_t *)0xE000E400 + ((irqnum) >> 2)) = (*((uint32_t *)0xE000E400 + ((irqnum) >> 2)) & (~(0xFF << (8 * ((irqnum) & 3))))) | (((priority) & 0xFF) << (8 * ((irqnum) & 3))))
#define NVIC_GET_PRIORITY(irqnum) (*((uint32_t *)0xE000E400 + ((irqnum) >> 2)) >> (8 * ((irqnum) & 3)) & 255)



#define UA(r) ((uint32_t) &(r))

typedef void (*isr)(void);

typedef struct {
	void* stackStart;
	isr reset;
	isr nmi;
	isr hardFault;
	isr _reserved1[7];
	isr svCall;
	isr _reserved2[2];
	isr pendSV;
	isr sysTick;
	isr dma[4];
	isr _reserved3;
	isr ftfa;
	isr lowVoltage;
	isr lowLeakageWakeup;
	isr i2c[2];
	isr spi[2];
	isr uart[3];
	isr adc;
	isr comparator;
	isr tpm[3];
	isr rtc;
	isr rtcSeconds;
	isr pit;
	isr i2s;
	isr usb;
	isr dac;
	isr tsi;
	isr mcg;
	isr lptmr;
	isr _reserved4;
	isr porta;
	isr portcd;
} NVICTable;


#endif //JULIMIN_CHIP_SUPPORT_H
