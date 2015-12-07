//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <math/fixed.h>

namespace nos {

template<class TVAL, int TPRECISION>
class NCO {
public:
	typedef fixed_point<TPRECISION> accum_t;
	typedef TVAL value_type;

protected:
	uint32_t sampleRate;
	uint32_t frequency = 0;

	accum_t accumulator = 0;
	accum_t phaseIncrement = 0;

	value_type *table;
	value_type value; // current value

public:
	NCO(uint32_t const &sampleRate, value_type *table) : sampleRate(sampleRate), table(table), value(table[0]) {

	}

	void setFrequency(uint32_t const &freq) {
		frequency = freq;
		phaseIncrement = accum_t((frequency << TPRECISION) / sampleRate);
	}

	void step() {
		accumulator += phaseIncrement;
		value = table[~(1 << TPRECISION + 1) & (accumulator >> TPRECISION)];
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
