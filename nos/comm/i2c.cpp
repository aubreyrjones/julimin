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
	_i2cInstances[instanceNumber] = this;

	SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
	i2cPort->F = I2C_F_MULT(0) | I2C_F_ICR(0x27); // pin it at 100kHz on the 48MHz PLL divider
	i2cPort->C1 = IDLE;

	_nvicTable.i2c[instanceNumber] = _i2cIRQ[instanceNumber];
	_mem_sync();

	if (instanceNumber == 0) {
		NVIC_EnableIRQ(I2C0_IRQn);
	}
	else if (instanceNumber == 1) {
		NVIC_EnableIRQ(I2C0_IRQn);
	}
}

I2CMaster::~I2CMaster() {
	NVIC_DisableIRQ(I2C0_IRQn);
	i2cPort->C1 = 0;
	SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;

	_i2cInstances[instanceNumber] = nullptr;
};


void I2CMaster::serviceIRQ() volatile {
	__disable_irq();
	uint8_t s = i2cPort->S;
	i2cPort->S |= I2C_S_IICIF_MASK;

	if (s & I2C_S_TCF_MASK) {
		if (i2cPort->C1 & I2C_C1_TX_MASK) {
			if (s & I2C_S_RXAK_MASK) {
				errorCode = ErrorCode::NO_ACK;
				errorCondition = true;
			}
		}
		else { // rx mode

			if (readCount) {
				i2cPort->C1 |= I2C_C1_TXAK_MASK; // acknowledge the next byte
				--readCount;
			}

			uint8_t readVal = i2cPort->D;
			if (!rxBuffer.put(readVal)) {
				errorCode = ErrorCode::OVERFLOW;
				errorCondition = true;
			}
		}
	}

	if (s & I2C_S_ARBL_MASK) {
		i2cPort->S |= I2C_S_ARBL_MASK;
		errorCode = ErrorCode::ARBITRATION;
		errorCondition = true;
	}
	__enable_irq();

	waitpoint.signal();
}



/** Read n registers. Blocking. Busy. */
bool I2CMaster::readRegisters(uint8_t const &slaveAddress, uint8_t const &reg, uint8_t *buf, size_t nRegisters) volatile {
	if (!start()) panic("1");
	if (!transmit(write_to(slaveAddress))) panic("2");
	if (!transmit(0)) panic("3");
	stop();

	nos::spin(10000);

	if (!start()) panic("4");
	if (!transmit(read_from(slaveAddress))) panic("5");

	if (!receive(buf, nRegisters)) panic("7");
	stop();

	console.write("\n\r-->");
	console.writeHex(buf[0]);
	console.write("<--\n\r");

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
		toggleStatusLEDState();
		waitpoint.wait();
		toggleStatusLEDState();
	}

	if (errorCondition) {
		console.write("i2c error: ");
		console.writeHex(static_cast<uint8_t>(errorCode));
		console.write("\n\r");
	}

	return !errorCondition;
}

bool I2CMaster::transmit(uint8_t b) volatile {
	waitpoint.reset();
	i2cPort->D = b;
	return wait();
}


bool I2CMaster::receive(uint8_t * d, size_t nBytes) volatile {
	if (!nBytes) return true;

	i2cPort->C1 = RECV;
	readCount += nBytes;

	// dummy read to kick it off
	uint8_t dummy = i2cPort->D;

	// wait for the receive to finish
	do {
		if (!wait()) return false;

		while (rxBuffer.get(*d)) {
			++d;
			--nBytes;
			if (!nBytes) return true;
		}

	} while (true);

	return true;
}

bool I2CMaster::stop() volatile {
	i2cPort->C1 = IDLE;
	return true;
}

}