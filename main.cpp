#include <math/fixed.h>
#include <core/led.h>
#include <core/delay.h>
#include <time/SystemTimer.h>
#include <time/NCO.h>

uint32_t table[] = {1, 0};

nos::NCO<uint32_t, 12> osc1(44100, table);

void blink() {
	static bool on = false;
	nos::setStatusLEDState((on = !on));
}

int main() {
	nos::SystemTimer timer(35, blink);

	return 0;
}
