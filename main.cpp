#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include "NCO.h"

void blink() {
	static bool on = false;
	nos::setStatusLEDState((on = !on));
}

int main() {
	nos::SystemTimer timer(35, blink);

	return 0;
}
