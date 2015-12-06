#include <kinetis.h>

int main() {

	PORTC_PCR5 |= PORT_PCR_MUX(1) | PORT_PCR_DSE;

	GPIOC_PDDR |= (1 << 5);

	GPIOC_PSOR = (1 << 5);

	return 0;
}
