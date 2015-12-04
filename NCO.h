//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <functional>
#include <fixed.h>

class NCO {
protected:
	nos::fixed accumulator {0.0f};
	nos::fixed phase {0.0f};

	uint16_t value = 0;
	std::function<uint16_t (nos::fixed const&)> vFunc;

public:

	void setFrequency(nos::fixed const& freq) {
		phase = freq * nos::fixed(256.0f) * nos::fixed(16777216.0f) + nos::fixed(0.5f);
	}

	void step() {
		uint8_t index = (accumulator.intValue >> 24) & 0xff;

		if (vFunc)
			value = vFunc(index);

		accumulator += phase;
	}

	uint16_t sample() {
		return value;
	}
};

#endif //JULIMIN_NCO_H
