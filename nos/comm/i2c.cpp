//
// Created by ARJ on 12/8/15.
//

#include <kl26z/chip_support.h>
#include <core/delay.h>
#include "i2c.h"
#include "uart.h"

namespace nos {

I2CMaster *_i2cInstances[NOS_N_I2C_INSTANCES] = {nullptr, nullptr};

void i2c0_irq() {
	if (_i2cInstances[0]) _i2cInstances[0]->serviceIRQ();
}

void i2c1_irq() {
	if (_i2cInstances[1]) _i2cInstances[1]->serviceIRQ();
}

static const VoidFunc _i2cIRQ[] = {i2c0_irq, i2c1_irq};


I2CMaster::I2CMaster(I2C_MemMapPtr port, uint32_t instanceNumber) : i2cPort(port), instanceNumber(instanceNumber) {

	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	i2cPort->F = I2C_F_MULT(0) | I2C_F_ICR(0x27); // pin it at 100kHz on the 48MHz PLL divider
	i2cPort->C1 = IDLE;

	_nvicTable.i2c[instanceNumber] = _i2cIRQ[instanceNumber];
	_mem_sync();
	NVIC_EnableIRQ(I2C0_IRQn);
}

I2CMaster::~I2CMaster() {
	NVIC_DisableIRQ(I2C0_IRQn);
	i2cPort->C1 = 0;
	SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;
};


void I2CMaster::serviceIRQ() volatile {

	uint8_t s = i2cPort->S;
	i2cPort->S |= I2C_S_IICIF_MASK;

	__disable_irq();
	if (s & I2C_S_TCF_MASK) {
		if (i2cPort->C1 & I2C_C1_TX_MASK) {
			if (s & I2C_S_RXAK_MASK) {
				errorCondition = true;
			}
		}
		else {
			bufferWord = i2cPort->D;
		}
	}

	if (s & I2C_S_ARBL_MASK) {
		errorCondition = true;
	}

	__enable_irq();

	waitpoint.signal();
}



/** Read n registers. Blocking. Busy. */
bool I2CMaster::readRegisters(uint8_t const &slaveAddress, uint8_t const &reg, uint8_t *buf, size_t nRegisters) volatile {
	if (!start()) panic("1");
	if (!transmit(write_to(32))) panic("2");
	if (!transmit(0)) panic("3");
	restart();
	if (!transmit(read_from(32))) panic("4");

	if (!receive(buf[0])) panic("5");

	panic("yo");

	return true;
}

bool I2CMaster::writeRegisters(uint8_t const &slaveAddress, uint8_t const &reg, uint8_t *buf,
							   size_t nRegisters) volatile {
	return false;
}

bool I2CMaster::start() volatile {
	i2cPort->C1 = START;

	while (!(i2cPort->S & I2C_S_BUSY_MASK)) { if (errorCondition) return false; };

	return true;
}

bool I2CMaster::wait() volatile {
	if (!errorCondition) {
		waitpoint.set();
		waitpoint.wait();
	}

	return !errorCondition;
}

bool I2CMaster::transmit(uint8_t b) volatile {
	i2cPort->D = b;
	return wait();
}

bool I2CMaster::restart() volatile {
	i2cPort->C1 = RESET_START;
	return !errorCondition;
}

bool I2CMaster::receive(uint8_t &d) volatile {
	i2cPort->C1 = RECV;

	uint8_t throwaway;
	throwaway = i2cPort->D; // fake read


	return wait();
}
}