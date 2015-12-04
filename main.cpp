#include "NCO.h"

nos::fixed idValFunc(nos::fixed const& c) {
	return fixedpoint::cos(c);
	return static_cast<uint16_t>((c.intValue >> 24) & 0xffff);
}

int main() {

	NCO<idValFunc> osc1;
	osc1.setFrequency(440.0f);

	volatile nos::fixed *i = (nos::fixed*) (0x0ffe);

	while (true) {
		osc1.step();
		*const_cast<nos::fixed*>(i) = osc1.sample();
	}

	return 0;
}
