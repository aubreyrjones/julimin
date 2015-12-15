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
}

I2CMaster::~I2CMaster() {
	i2cPort->C1 = 0;


	SIM_SCGC4 &= ~SIM_SCGC4_I2C0_MASK;

};

void I2CMaster::s() volatile {
	while (i2cPort->S & I2C_S_BUSY_MASK) {};

	i2cPort->C1 = START;

	while (! (i2cPort->S & I2C_S_BUSY_MASK)) {}
}

void I2CMaster::rs() volatile {
	i2cPort->C1 = RESET_START;
}


//bool I2CMaster::addressSlave(uint8_t const &addr) volatile {
//	i2cPort->D = addr;
//
//	while (true) {
//		uint8_t s = i2cPort->S;
//		if (s & I2C_S_TCF_MASK){
//			return true;
//		}
//
//		if (s & I2C_S_RXAK_MASK) {
//			panic("8");
//			return false;
//		}
//
//		if (s & I2C_S_ARBL_MASK) {
//			panic("9");
//			return false;
//		}
//	}
//}


bool I2CMaster::tx(uint8_t const &v) volatile {
	i2cPort->D = v;
	while (true) {
		uint8_t s = i2cPort->S;
		console.writeHex(i2cPort->S);
		console.write(" ");


		if (s & I2C_S_TCF_MASK) {
			return true;
		}

		if (s & I2C_S_RXAK_MASK) {
			panic("6");
			return false;
		}

		if (s & I2C_S_ARBL_MASK) {
			panic("7");
			return false;
		}
	}
}


void I2CMaster::bogus_rx() volatile {
	uint8_t volatile v = i2cPort->D;
	v = v;
}

bool I2CMaster::rx(uint8_t & v) volatile {
	while (true) {
		uint8_t s = i2cPort->S;
		if (s & I2C_S_TCF_MASK){
			break;
		}
		if (s & (I2C_S_RXAK_MASK | I2C_S_ARBL_MASK)) {
			panic("5");
			return false;
		}
	}
	v = i2cPort->D;
	return true;
}

void I2CMaster::e() volatile {
	i2cPort->C1 = IDLE;
}

void I2CMaster::srecv() volatile {
	i2cPort->C1 = RECV;
}

/** Read n registers. Blocking. Busy. */
bool I2CMaster::readRegisters(uint8_t const &slaveAddress, uint8_t const &reg, uint8_t *buf,
							  size_t nRegisters) volatile {
	s();
	if (!tx(write_to(slaveAddress))) { // address
		panic("1");
		return false;
	}
	console.write("1 ");

	if (!tx(reg)) { // write register
		panic("2");
		return false;
	}
	console.write("2 ");

	e();
	spin(1000);
	s();
	//rs(); // restart

	if (!tx(read_from(slaveAddress))) { // address again
		panic("3");
		return false;
	}
	console.write("3 ");

	srecv(); // start receive

	bogus_rx();

	for (uint8_t *end = buf + nRegisters; buf != end; buf++) {
		uint8_t v;
		if (!rx(v)) {
			panic("4");
			return false;
		}
		console.write("4 ");
		*buf = v;
	}

	e();

	console.write("5 ");

	return true;
}

bool I2CMaster::writeRegisters(uint8_t const &slaveAddress, uint8_t const &reg, uint8_t *buf,
							   size_t nRegisters) volatile {
	return false;
}

}