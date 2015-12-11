//
// Created by ARJ on 12/8/15.
//

#ifndef JULIMIN_I2C_H
#define JULIMIN_I2C_H

#include <core/common.h>
#include <core/chip.h>

namespace nos {

class I2CMaster {
protected:
	I2C_Type volatile* i2cPort;

public:
	I2CMaster(I2C_MemMapPtr port) : i2cPort(port) {
		SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
		i2cPort->F = I2C_F_MULT(0) | I2C_F_ICR(27); // pin it at 100kHz
		i2cPort->C1 = I2C_C1_IICEN_MASK;
	}

	~I2CMaster() {

	};

	/** Read n registers. Blocking. Busy. */
	bool readRegister(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) {

		return false;
	}

	/** Write n registers. Blocking. Busy. */
	bool writeRegister(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) {
		return false;
	}
};

}

#endif //JULIMIN_I2C_H
