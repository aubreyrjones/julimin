#include <kinetis.h>

#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include "NCO.h"

nos::fixed sineFunc(nos::fixed const& phase) {
	return fixedpoint::sin(phase);
}

nos::fixed cosFunc(nos::fixed const& phase) {
	return fixedpoint::cos(phase);
}

nos::fixed idFunc(nos::fixed const& phase) {
	return phase;
}

NCO o1(sineFunc);
NCO o2(cosFunc);
NCO o3(idFunc);

int main() {

	nos::fixed volatile something;

	o1.setFrequency(440_f);
	o2.setFrequency(220_f);

	while (true) {
		o1.step();
		o2.step();
		o3.step();

		const_cast<nos::fixed&>(something) = (o1.sample() + o2.sample()) / 2_f;
		o1.setFrequency(o1.getFrequency() + 0.1_f);
		o2.setFrequency(o1.getFrequency() / 2_f);

		if (o1.sample() > 0.5_f) {
			o3.setFrequency(o1.getFrequency() / 2_f + 2.34_f);
			nos::setStatusLEDState(true);
		}
		else {
			nos::setStatusLEDState(false);
		}
	}

	return 0;
}
