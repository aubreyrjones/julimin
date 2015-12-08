#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>
#include "tables/voices.h"

nos::NCO<uint16_t, 9> osc1(44100, &voice1[0]);

void blink() {
	static bool on = false;
	nos::setStatusLEDState((on = !on));
}

int main() {
	nos::SystemTimer timer(35, blink);

	return 0;
}
