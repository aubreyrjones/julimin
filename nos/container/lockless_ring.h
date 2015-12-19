/* 
 * File:   lockless_ring.h
 * Author: netzapper
 *
 * Created on December 16, 2013, 10:02 PM
 */

#include <core/chip.h>
#include <core/errors.h>

#ifndef LOCKLESS_RING_H
#define	LOCKLESS_RING_H

namespace nos {

	template <typename T, uint32_t capacity>
	class LocklessRingQueue 
	{
	private:
		T volatile buffer[capacity + 1];
		uint32_t volatile back = 0;
		uint32_t volatile front = 0;
		
		inline uint32_t modcap(uint32_t i) const volatile
		{
			return i % capacity;
		}
		
		inline bool modcap_eq(uint32_t a, uint32_t b) const volatile
		{
			return modcap(a) == modcap(b);
		}

		inline bool _empty() const volatile {
			return modcap_eq(front, back);
		}

		inline bool _full() const volatile {
			return modcap_eq(front + 1, back);
		}

	public:

		inline bool empty() const volatile
		{
			NOS_NOIRQ;
			return _empty();
		}

		inline bool full() const volatile
		{
			NOS_NOIRQ;
			return _full();
		}
		
		uint32_t getSize() const volatile
		{
			NOS_NOIRQ;
			return front - back;
		}


		bool put(T const& value) volatile {
			NOS_NOIRQ;

			if (_full()) {
				return false; //full
			}

			buffer[modcap(front)] = value;
			++front;
			_mem_barrier();
			return true;
		}

		bool put_b(T const& value) volatile {
			restart:
			while (full()) { }

			{
				NOS_NOIRQ;
				if (_full()) {
					goto restart;
				}

				buffer[modcap(front)] = value;
				++front;
				_mem_barrier();
				return true;
			}
		}


		bool get(T & out) volatile {
			NOS_NOIRQ;

			if (_empty()) {
				return false;
			}

			out = buffer[modcap(back)];
			++back;
			_mem_barrier();
			return true;
		}

		void get_b(T & out) volatile {
			restart:
			while (empty()) { }

			{
				NOS_NOIRQ;
				if (_empty())
					goto restart;

				out = buffer[modcap(back)];
				++back;
				_mem_barrier();
			}
		}
	};
}
#endif	/* LOCKLESS_RING_H */

