//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <core/chip.h>

namespace nos {

/**
 * Implements a numerically-controller oscillator. Call `step()` every `1/sampleRate` seconds, and this will sample
 * the fixed-size table of size `2**TBITS` in real time, sampling the entire table in `1/frequency` seconds.
 *
 * While `setFrequency()` uses floating point operations to calculate internal values, the `step()` and `sample()`
 * functions are integer-only and suitable for use inside ISRs.
 * */
template<class TVAL, uint32_t TBITS>
class NCO {
public:
	typedef TVAL value_type;
	static constexpr uint32_t intBits = TBITS;
	static constexpr uint32_t fractBits = 32 - TBITS;
	static constexpr float intBitsFloat = positive_power(2, intBits);
	static constexpr float fractBitsFloat = positive_power(2, fractBits);
	static constexpr float invIntBitsFloat = 1.0f / fractBitsFloat;

protected:
	uint32_t sampleRate;
	float frequency = 0.0f;

	uint32_t accumulator = 0;
	uint32_t phaseIncrement = 0;

	value_type const* table;
	value_type value; // current value

public:

	NCO(uint32_t const &sampleRate, value_type const* sampleTable) :
			sampleRate(sampleRate),
			table(sampleTable),
			value(sampleTable[0])
	{ }

	/**
	 * Set the frequency at which to sample. Use of float is unfortunate, but expedient to cover all possible
	 * supported frequencies.*/
	void setFrequency(float const& freq) volatile {
		frequency = freq;

		float phIncr = (frequency * intBitsFloat) / sampleRate;

		phaseIncrement = static_cast<uint32_t>(phIncr * fractBitsFloat);

		_mem_barrier();
	}

	float const& getFrequency() {
		return frequency;
	}

	/** Step function, integer-only, suitable for use in a real-time ISR. */
	void step() volatile {
		accumulator += phaseIncrement;
		value = table[bitmask(intBits) & (accumulator >> fractBits)];
		_mem_barrier();
	}

	/** Return the current table value. */
	value_type const& sample() volatile const  {
		return const_cast<value_type const&>(value);
	}
};

}

#endif //JULIMIN_NCO_H
