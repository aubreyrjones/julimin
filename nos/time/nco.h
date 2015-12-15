//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

#include <core/chip.h>

namespace nos {

/**
 * Implements a numerically-controlled oscillator. Call `step()` every `1/sampleRate` seconds.
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

protected:
	uint32_t sampleRate;
	float const sampleRateConstant;
	nos::fixed frequency = 0.0_f;

	uint32_t accumulator = 0;
	uint32_t phaseIncrement = 0;

	uint32_t index = 0; // current index

public:

	NCO(uint32_t const &sampleRate) :
			sampleRate(sampleRate),
			sampleRateConstant(intBitsFloat / sampleRate)
	{ }

	/**
	 * Set the frequency at which to sample. Makes internal use of fixed-to-float conversion. */
	void setFrequency(nos::fixed const& freq) volatile {
		*const_cast<nos::fixed*>(&frequency) = freq;

		float floatFreq = fixedpoint::fix2float<nos::fixed::precision>(freq.intValue);

		float phIncr = floatFreq * sampleRateConstant;

		phaseIncrement = static_cast<uint32_t>(phIncr * fractBitsFloat);

		_mem_barrier();
	}

	nos::fixed const& getFrequency() {
		return frequency;
	}

	/** Step function, integer-only, suitable for use in a real-time ISR. */
	void step() volatile {
		accumulator += phaseIncrement;
		index = bitmask(intBits) & (accumulator >> fractBits);
		_mem_barrier();
	}

	uint32_t const volatile& getIndex() volatile const { return index; }

	uint32_t const volatile& operator*() volatile const { return getIndex(); }
};

}

#endif //JULIMIN_NCO_H
