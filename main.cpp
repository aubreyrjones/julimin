#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/nco.h>
#include <core/errors.h>
#include <analog/dac.h>
#include "tables/voices.h"

#include <fsl_i2c_master_driver.h>

constexpr uint32_t SAMPLE_RATE = 44100;

nos::dac dac1(DAC0_BASE_PTR);
nos::NCO<uint16_t, 9> osc1(SAMPLE_RATE);


void ncoblink() {
	osc1.step();
	dac1.set(voice1[*osc1]);
}

int main() {

	i2c_master_state_t i2cMaster;
	i2c_device_t device = {
			.address = 0x7fU,
			.baudRate_kbps = 400
	};

	I2C_DRV_MasterInit(0, &i2cMaster);

	osc1.setFrequency(440.0f);

	nos::setStatusLEDState(true);

	nos::SystemTimer volatile timer(SAMPLE_RATE, ncoblink);

	for(;;) {};

	return 0;
}

