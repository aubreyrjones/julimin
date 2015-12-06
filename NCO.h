//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <math/fixed.h>

typedef nos::fixed (* ValueFunction)(nos::fixed const&);

class NCO {
protected:
	nos::fixed frequency = 0_f;
	nos::fixed accumulator = 0_f;
	nos::fixed phase = 0_f;

	nos::fixed value = 0_f;

	ValueFunction valFunc;

public:

	NCO(ValueFunction const& func) : valFunc(func) {
		value = valFunc(accumulator);
	}

	void setFrequency(nos::fixed const& freq) {
		frequency = freq;
		phase = freq * 256_f * 16777216_f + 0.5_f;
	}

	void step() {
		accumulator += phase;
		value = valFunc(accumulator);
	}

	nos::fixed sample() {
		return value;
	}

	nos::fixed const& getFrequency() {
		return frequency;
	}
};

#endif //JULIMIN_NCO_H
