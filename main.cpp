#include "NCO.h"

int main() {

	NCO osc1;
	osc1.setFrequency(440.0f);

	volatile int *i = (int*) (0x0ffe);

	while (true) {
		osc1.step();
		*i = osc1.sample();
	}

	return 0;
}
