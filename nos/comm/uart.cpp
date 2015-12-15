//
// Created by ARJ on 12/9/15.
//


#include "uart.h"

#include <core/chip.h>
#include <core/startup.h>
#include <core/errors.h>


namespace nos {

/// storage for the console UART. This will be constructed after clocks and LEDs are online, but before the call to
// main().
Console volatile console {115200};

void consoleISR() {
	console.serviceInterrupt();
}

static constexpr uint32_t TX_IDLE = UART0_C2_TE_MASK;
static constexpr uint32_t TX_SENDING = UART0_C2_TE_MASK | UART0_C2_TIE_MASK;
static constexpr uint32_t TX_FINISHING = UART0_C2_TE_MASK | UART0_C2_TCIE_MASK;

void Console::serviceInterrupt() volatile {

	if (UART0_S1 & UART0_S1_TDRE_MASK) {
		uint8_t c;
		if (txBuffer.get(c)) {
			UART0_D = c;
			if (txBuffer.getSize() == 1) {
				UART0_C2 = TX_FINISHING;
			}
		}
	}

	if (UART0_S1 & UART0_S1_TC_MASK) {
		txActive = false;
		UART0_C2 = TX_IDLE;
	}
}

Console::Console(uint32_t baudrate) : baud(baudrate) {
	SIM_SCGC4 |= SIM_SCGC4_UART0_MASK;

	uint32_t targetBaudDivisor = CONSOLE_DEVICE_BUS_FREQUENCY / (baudrate * 16);

	UART0_BDL = static_cast<uint8_t>(0xff & (targetBaudDivisor));
	UART0_BDH = static_cast<uint8_t>(0x1f & (targetBaudDivisor >> (3 + 8)));
	UART0_C1 = 0; //UART0_C1_PE_MASK;

	_nvicTable.uart[0] = consoleISR;
	_mem_sync();

	UART0_C2 = UART0_C2_TE_MASK;
	NVIC_EnableIRQ(UART0_IRQn);
}

Console::~Console() {

	UART0_C2 = 0;
	NVIC_DisableIRQ(UART0_IRQn);
	_nvicTable.uart[0] = nullptr;
	SIM_SCGC4 &= ~SIM_SCGC4_UART0_MASK;
}


void Console::startTX() volatile {
	uint8_t c;
	if (txBuffer.get(c)) {
		UART0_D = c;
		UART0_C2 = TX_SENDING;
	}
}

void Console::requestTX() volatile {
	if (!txActive) {
		txActive = true;

		startTX();
	}
}


uint32_t Console::write(uint8_t const *data, uint32_t len) volatile {
	uint32_t written = 0;
	while (len && txBuffer.put(data[written])) {
		--len;
		++written;
	}

	if (written) {
		requestTX();
	}

	return written;
}


uint32_t Console::write(char const *data, uint32_t len) volatile {
	return write(reinterpret_cast<uint8_t const*>(data), len);
}

void Console::write(char const* str) volatile {
	while (*str) {
		if (txBuffer.put(*str)){
			++str;
		}
		else {
			requestTX(); // try to clear the buffer
		}
	}

	requestTX();
}


void Console::writeLine(char const *str) volatile {
	write(str);
	write("\n\r");
}

static const uint8_t hexConvTable[] =
		{'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void Console::writeHex(uint8_t byte) volatile {
	while (!txBuffer.put(hexConvTable[0x0f & (byte >> 4)])){
		requestTX();
	}
	while (!txBuffer.put(hexConvTable[0x0f & byte ])){
		requestTX();
	}
}

}