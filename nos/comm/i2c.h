//
// Created by ARJ on 12/8/15.
//

#ifndef JULIMIN_I2C_H
#define JULIMIN_I2C_H

#include <core/common.h>
#include <core/chip.h>
#include <core/errors.h>
#include <core/sync.h>

namespace nos {

class I2CMaster {
protected:

	Semaphore semaphore {1};

	static constexpr uint8_t WRITE = 0;
	static constexpr uint8_t READ = 1;

	static constexpr uint8_t IDLE = I2C_C1_IICEN_MASK;
	static constexpr uint8_t START = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK | I2C_C1_TX_MASK;
	static constexpr uint8_t RESET_START = I2C_C1_IICEN_MASK |  I2C_C1_MST_MASK | I2C_C1_TX_MASK | I2C_C1_RSTA_MASK ;
	static constexpr uint8_t RECV = I2C_C1_IICEN_MASK | I2C_C1_MST_MASK;

	I2C_Type volatile* i2cPort;

	bool waitTransferComplete() volatile {
		while (true) {
			uint8_t s = i2cPort->S;
			if (s & I2C_S_TCF_MASK) {
				return true;
			}
			if (s & I2C_S_RXAK_MASK) {
				return false;
			}
		};
	}

	bool addressRegister(uint8_t const& slaveAddress, uint8_t const& reg) volatile {
		i2cPort->C1 = START; // go to master mode and start

		i2cPort->D = slaveAddress << 1 | WRITE; // send address, write
		if (!waitTransferComplete()) {
			return false;
		}

		i2cPort->D = reg;

		return waitTransferComplete();
	}

public:
	I2CMaster(I2C_MemMapPtr port) : i2cPort(port) {
		SIM_SCGC4 |= SIM_SCGC4_I2C0_MASK;
		i2cPort->F = I2C_F_MULT(0) | I2C_F_ICR(27); // pin it at 100kHz
		i2cPort->C1 = IDLE;
	}

	~I2CMaster() {

	};


	/** Read n registers. Blocking. Busy. */
	bool readRegisters(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) volatile {
		if (!addressRegister(slaveAddress, reg)) {
			return false;
		}

		i2cPort->C1 = RESET_START;
		i2cPort->D = (slaveAddress << 1) | READ;
		if (!waitTransferComplete()) {
			panic("3");
		}

		i2cPort->C1 = RECV;
		for (uint8_t *end = buf +nRegisters; buf != end; buf++){
			*(buf++) = i2cPort->D;
			if (!waitTransferComplete()){
				panic("4");
				return false;
			}
		}

		i2cPort->C1 = IDLE;

		return true;
	}

	/** Write n registers. Blocking. Busy. */
	bool writeRegisters(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) volatile {
		return false;
	}
};

}

#endif //JULIMIN_I2C_H
