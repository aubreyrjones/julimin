//
// Created by ARJ on 12/9/15.
//

#ifndef JULIMIN_UART_H
#define JULIMIN_UART_H

#include <stdint.h>
#include <container/lockless_ring.h>

namespace nos {

class Console {
	friend void consoleISR();

protected:
	LocklessRingQueue<uint8_t, 128> txBuffer;

	bool volatile txActive = false;

	uint32_t baud;

	void serviceInterrupt() volatile;
	void startTX() volatile;

	void requestTX() volatile;

public:
	Console(uint32_t baudrate);
	~Console();

	/** Write `size` characters from `data`. */
	uint32_t write(uint8_t const* data, uint32_t len) volatile;

	uint32_t write(char const* data, uint32_t len) volatile;

	void write(char const* str) volatile;

	void writeLine(char const* str) volatile;

	void writeHex(uint8_t byte) volatile;

	void writeHex(uint32_t word) volatile;
};

extern Console volatile console;

}

#endif //JULIMIN_UART_H
