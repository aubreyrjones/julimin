#include "NCO.h"

uint16_t idValFunc(nos::fixed const& c) {
	return static_cast<uint16_t>((c.intValue >> 24) & 0xffff);
}

int main() {

	NCO<idValFunc> osc1;
	osc1.setFrequency(440.0f);

	volatile int *i = (int*) (0x0ffe);

	while (true) {
		osc1.step();
		*i = osc1.sample();
	}

	return 0;
}
