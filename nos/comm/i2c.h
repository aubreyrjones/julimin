//
// Created by ARJ on 12/8/15.
//

#ifndef JULIMIN_I2C_H
#define JULIMIN_I2C_H

#include <core/common.h>
#include <core/chip.h>
#include <core/errors.h>
#include <core/sync.h>
#include <container/lockless_ring.h>

namespace nos {

class I2CMaster {
protected:
	enum class ErrorCode : uint8_t {
		NONE = 0,
		ARBITRATION = 1,
		NO_ACK = 2,
		UNDERFLOW = 3,
		OVERFLOW = 4
	};

	static constexpr uint8_t WRITE = 0;
	static constexpr uint8_t READ = 1;

	static inline uint8_t write_to(uint8_t const& addr) { return (addr << 1) | WRITE; }
	static inline uint8_t read_from(uint8_t const& addr) { return (addr << 1) | READ; }

	static constexpr uint8_t IDLE = I2C_C1_IICEN_MASK;
	static constexpr uint8_t START = I2C_C1_IICEN_MASK | I2C_C1_IICIE_MASK | I2C_C1_MST_MASK | I2C_C1_TX_MASK;
	static constexpr uint8_t RECV = I2C_C1_IICEN_MASK |I2C_C1_IICIE_MASK | I2C_C1_MST_MASK;

	I2C_Type volatile* i2cPort;
	uint32_t instanceNumber;
	WaitPoint waitpoint {};

	size_t volatile readCount = 0;
	LocklessRingQueue<uint8_t, 64> rxBuffer {};
	bool errorCondition = false;
	ErrorCode errorCode {ErrorCode::NONE};

	bool wait() volatile;

	bool start() volatile;
	bool transmit(uint8_t b) volatile;
	bool receive(uint8_t * d, size_t nBytes) volatile;
	bool stop() volatile;

public:
	I2CMaster(I2C_MemMapPtr port, uint32_t instanceNumber);

	~I2CMaster();

	void serviceIRQ() volatile;

	/** Read n registers. Blocking. Busy. */
	bool readRegisters(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) volatile;

	/** Write n registers. Blocking. Busy. */
	bool writeRegisters(uint8_t const& slaveAddress, uint8_t const& reg, uint8_t *buf, size_t nRegisters = 1) volatile;
};

}

#endif //JULIMIN_I2C_H
