//
// Created by ARJ on 12/10/15.
//

#ifndef JULIMIN_SYNC_H
#define JULIMIN_SYNC_H

#include "chip.h"
#include <atomic>
#include <list>

namespace nos {

class Spinlock {
protected:
	std::atomic<bool> _lock {false};

public:

	void lock() {
		bool result = false;
		while (!_lock.compare_exchange_strong(result, true, std::memory_order_acquire, std::memory_order_relaxed)) {}
	}

	void unlock() {
		_lock.store(false, std::memory_order_release);
	}
};

class WaitPoint {
protected:
	std::atomic<bool> _lock {false};

public:
	void set() volatile {
		_lock = false;
	}

	void wait() volatile {
		bool result = true;
		while (!_lock.compare_exchange_strong(result, false)) {}
	}

	void signal() volatile {
		_lock.store(true);
	}

};

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

		count++;
	}
};

}

#endif //JULIMIN_SYNC_H
