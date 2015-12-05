#include "NCO.h"

nos::fixed cosValFunc(nos::fixed const &c) {
	return fixedpoint::cos(c);
}

nos::fixed sinValFunc(nos::fixed const& c) {
	return fixedpoint::sin(c);
}

NCO<cosValFunc> osc1;
NCO<sinValFunc> osc2;

int main() {
	osc1.setFrequency(440.0f);
	osc2.setFrequency(220.0f);

	volatile nos::fixed *i = (nos::fixed*) (0x0ffe);

	while (true) {
		osc1.step();
		osc2.step();
		*const_cast<nos::fixed*>(i) = (osc1.sample() + osc2.sample()) / nos::fixed(2.0f);
		osc1.setFrequency(osc2.getFrequency() + nos::fixed(1.0f));
		osc2.setFrequency(osc1.getFrequency() / nos::fixed(2.0f));
	}

	return 0;
}
