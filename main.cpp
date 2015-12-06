#include "NCO.h"

nos::fixed cosValFunc(nos::fixed const &c) {
	return fixedpoint::cos(c);
}

nos::fixed sinValFunc(nos::fixed const& c) {
	return fixedpoint::sin(c);
}

NCO osc1(cosValFunc);
NCO osc2(sinValFunc);

int main() {
	osc1.setFrequency(440_f);
	osc2.setFrequency(220_f);

	volatile nos::fixed *i = (nos::fixed*) (0x0ffe);

	while (true) {
		osc1.step();
		osc2.step();
		*const_cast<nos::fixed*>(i) = (osc1.sample() + osc2.sample()) / 2_f;
		osc1.setFrequency(osc2.getFrequency() + 2_f);
		osc2.setFrequency(osc1.getFrequency() / 2_f);
	}

	return 0;
}
