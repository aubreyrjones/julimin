//
// Created by ARJ on 12/8/15.
//

#ifndef JULIMIN_DAC_H
#define JULIMIN_DAC_H

#include <core/chip.h>
#include <core/common.h>

namespace nos {

class DAC {
protected:
	volatile DAC_Type* dacPort;

public:
	DAC(DAC_MemMapPtr dacPort) : dacPort(dacPort) {
		SIM_SCGC6 |= SIM_SCGC6_DAC0_MASK;
		dacPort->C0 |= DAC_C0_DACEN_MASK | DAC_C0_DACRFS_MASK;
	}

	~DAC() {
		dacPort->C0 = 0; // disable everything
		SIM_SCGC6 &= ~SIM_SCGC6_DAC0_MASK;
	}

	inline void set(uint32_t const& value) volatile {
		dacPort->DAT[0].DATL = static_cast<uint8_t>(0xff & value);
		dacPort->DAT[0].DATH = static_cast<uint8_t>(0x0f & value >> 8);
	}
};

}

#endif //JULIMIN_DAC_H
