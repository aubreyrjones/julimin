#include <kinetis.h>

int main() {
	PORTC_PCR5 = PORT_PCR_MUX(1) | PORT_PCR_DSE | PORT_PCR_SRE;

	GPIOC_PDDR |= (1 << 5);

	while (true) {
		GPIOC_PSOR = (1 << 5);
		uint32_t ticksleft = 1500000;
		while (ticksleft != 0) {
			ticksleft--;
			__asm__ volatile ("nop");
		}
		GPIOC_PCOR = (1 << 5);
		ticksleft = 1500000;
		while (ticksleft != 0) {
			ticksleft--;
			__asm__ volatile ("nop");
		}
	}

	return 0;
}
