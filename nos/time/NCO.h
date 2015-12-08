//
// Created by ARJ on 12/3/15.
//

#ifndef JULIMIN_NCO_H
#define JULIMIN_NCO_H

namespace nos {



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
	NCO(uint32_t const &sampleRate, value_type const* table) : sampleRate(sampleRate), table(table), value(table[0]) {

	}

	void setFrequency(float const& freq) {
		// there's no fixed point base that covers this all, leave it to the float emulation, luckily this isn't a
		// tight loop. The actual synthesis is happening in integer land.
		frequency = freq;

		float phIncr = (frequency * intBitsFloat) / sampleRate;

		phaseIncrement = static_cast<uint32_t>(phIncr * fractBitsFloat);
	}

	void step() {
		accumulator += phaseIncrement;
		value = table[bitmask(intBits) & (accumulator >> fractBits)];
	}

	value_type sample() {
		return value;
	}

	float const& getFrequency() {
		return frequency;
	}
};

}

#endif //JULIMIN_NCO_H
