//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <math/fixed.h>

template <uint16_t (*VAL_FUNC)(nos::fixed const&)>
class NCO {
protected:
	nos::fixed accumulator {0.0f};
	nos::fixed phase {0.0f};

	uint16_t value = 0;

public:

	void setFrequency(nos::fixed const& freq) {
		phase = freq * nos::fixed(256.0f) * nos::fixed(16777216.0f) + nos::fixed(0.5f);
	}

	void step() {
		accumulator += phase;
		value = VAL_FUNC(accumulator);
	}

	uint16_t sample() {
		return value;
	}
};

#endif //JULIMIN_NCO_H
