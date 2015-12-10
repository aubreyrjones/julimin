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
	I2CMaster(I2C_MemMapPtr port, uint32_t baud) : i2cPort(port) {

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
