//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <math/fixed.h>

namespace nos {

template<class TVAL, int TBITS>
class NCO {
public:
	typedef fixed_point<32 - TBITS> accum_t;
	typedef TVAL value_type;

protected:
	uint32_t sampleRate;
	uint32_t frequency = 0;

	accum_t accumulator = 0;
	accum_t phaseIncrement = 0;

	value_type const* table;
	value_type value; // current value

public:
	NCO(uint32_t const &sampleRate, value_type const* table) : sampleRate(sampleRate), table(table), value(table[0]) {

	}

	void setFrequency(uint32_t const &freq) {
		frequency = freq;
		phaseIncrement = accum_t((frequency << TBITS) / sampleRate);
	}

	void step() {
		accumulator += phaseIncrement;
		value = table[accumulator.integerPart()];
	}

	value_type sample() {
		return value;
	}

	uint32_t const &getFrequency() {
		return frequency;
	}
};

}

#endif //JULIMIN_NCO_H
