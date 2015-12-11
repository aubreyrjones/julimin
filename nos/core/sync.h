//
// Created by ARJ on 12/10/15.
//

#ifndef JULIMIN_SYNC_H
#define JULIMIN_SYNC_H

#include "chip.h"

namespace nos {

class Semaphore {
protected:
	int const max;
	int volatile count;

public:
	Semaphore(int max = 1) : max(max), count(max) {}

	bool lock() volatile {
		_top:
		__disable_irq();

		if (count > 0){
			--count;
			__enable_irq();
			return true;
		}
		__enable_irq();

		while (count < 1) {};
		goto _top;
	}

	void signal() volatile {
		NOS_NOIRQ;

		if (count < max) {
			count++;
		}
	}
};

}

#endif //JULIMIN_SYNC_H
